#include"core/vec.hpp"
#include"core/mat.hpp"
#include"core/bound.hpp"
#include"core/quat.hpp"
#include"core/ray.hpp"
#include"camera/pose.hpp"
#include"color/rgb.hpp"
#include"film/filter.hpp"
#include"film/film.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include"test/doctest.h"
#include<random>
#include<cstdio>
#include"util/transform.hpp"
#include"sample/Independent.hpp"
using namespace fsytd;
std::mt19937 rg(std::random_device{}());
std::uniform_real_distribution<float>dis(0.1f,3.14158265f),dis1(0.0f,1.0f);
bool f(auto a,auto b){int x,y;bool ck=1;for(x=0;x<3;x++)for(y=0;y<4;y++)if(std::abs(a.mat[x][y]-b.mat[x][y])>1e-5)return 0;return 1;}
TEST_CASE("dot and cross"){
int x;for(x=1;x<=100000;x++){
vec3<float,V>a(dis(rg),dis(rg),dis(rg)),b(dis(rg),dis(rg),dis(rg));
auto ab=cs(a,b);
CHECK(std::abs(dot(ab,a))<1e-5f);
//printf("%f %f %f\n",a.x,a.y,a.z);
//printf("%f %f %f\n",b.x,b.y,b.z);
//printf("%f %f %f\n",ab.x,ab.y,ab.z);
//printf("%f\n",dot(ab,a));
}}
TEST_CASE("mat decompose"){
int x;for(x=1;x<=100000;x++){
vec3<float,V>t0(dis(rg),dis(rg),dis(rg));
quat<float>q0=quat<float>::axis(nor(vec3<float,V>(dis(rg),dis(rg),dis(rg))),dis(rg));
vec3<float,V>s0(dis(rg),dis(rg),dis(rg));mat34<float>m=mat34<float>::trs(t0,q0,s0);
vec3<float,V>t1,s1;quat<float>q1;
mat34<float>::decompose(m,t1,q1,s1);
//printf("%f %f %f %f\n",q0.w,q0.x,q0.y,q0.z);
//printf("%f %f %f %f\n",q1.w,q1.x,q1.y,q1.z);
//printf("%f %f %f\n",s0.x,s0.y,s0.z);
//printf("%f %f %f\n",s1.x,s1.y,s1.z);
CHECK(quat<float>::qlen(q1-q0)<=1e-5f);
CHECK(len(t1-t0)<=1e-5f);
CHECK(len(s1-s0)<=1e-5f);}}
TEST_CASE("mat trs"){
int x;for(x=1;x<=100000;x++){
quat<float>q=quat<float>::axis(nor(vec3<float,V>(dis(rg),dis(rg),dis(rg))),dis(rg));
mat34<float>m=mat34<float>::rot(q);
quat<float>q1=quat<float>::matr(m);
CHECK(quat<float>::qlen(q-q1)<=1e-5f);}}
TEST_CASE("trace"){
int x;for(x=1;x<=100000;x++){
quat<float>q=quat<float>::axis(nor(vec3<float,V>(dis(rg),dis(rg),dis(rg))),3.14158265f);
mat34<float>r=mat34<float>::rot(q);
quat<float>q1=quat<float>::matr(r);
CHECK(quat<float>::qlen(q1-q)<=1e-5);}}
TEST_CASE("unit"){
mat34<float>m=mat34<float>::unit();
quat<float>q=quat<float>::matr(m);
CHECK(quat<float>::qlen(q-quat<float>::unit())<=1e-5f);}
TEST_CASE("decompose_negative"){
int x;for(x=1;x<=100000;x++){
vec3<float,V>t0(dis(rg),dis(rg),dis(rg));
quat<float>q0=quat<float>::axis(nor(vec3<float,V>(dis(rg),dis(rg),dis(rg))),dis(rg));
vec3<float,V>s0(-dis(rg),-dis(rg),-dis(rg));
mat34<float>m1=mat34<float>::trs(t0,q0,s0);
vec3<float,V>t1,s1;quat<float>q1;mat34<float>::decompose(m1,t1,q1,s1);
mat34<float>m2=mat34<float>::trs(t1,q1,s1);
int i,j;for(i=0;i<3;i++){for(j=0;j<4;j++)CHECK(m1.mat[i][j]-m2.mat[i][j]<=1e-5);}}}
TEST_CASE("matr_mv"){
int x;for(x=1;x<=100000;x++){
float a=dis(rg),b=dis(rg),c=dis(rg);
mat34<float>m=mat34<float>::mv(vec3<float,V>(a,b,c));
vec3<float,V>t1,s1;quat<float>q1;mat34<float>::decompose(m,t1,q1,s1);
//printf("%f %f %f\n",t1.x,t1.y,t1.z);
//printf("%f %f %f\n",a,b,c);
//printf("%f\n",len(vec3<float,V>(a,b,c)-t1));
//printf("%f %f %f\n",s1.x,s1.y,s1.z);
CHECK(quat<float>::qlen(q1-quat<float>::unit())<=1e-5);
CHECK(len(vec3<float,V>(a,b,c)-t1)<=1e-5);
CHECK(len(vec3<float,V>(1,1,1)-s1)<=1e-5);}}
TEST_CASE("matr_scale"){
int x;for(x=1;x<=100000;x++){
float a=dis(rg),b=dis(rg),c=dis(rg);
mat34<float>m=mat34<float>::scale(vec3<float,V>(a,b,c));
vec3<float,V>t1,s1;quat<float>q1;mat34<float>::decompose(m,t1,q1,s1);
CHECK(quat<float>::qlen(q1-quat<float>::unit())<=1e-5);
CHECK(len(t1)<=1e-5);
CHECK(len(vec3<float,V>(a,b,c)-s1)<=1e-5);}}
TEST_CASE("slrep"){
int x;for(x=1;x<=100000;x++){
quat<float>q1=quat<float>::axis(vec3<float,V>(dis(rg),dis(rg),dis(rg)),dis(rg));
quat<float>q2=quat<float>::axis(vec3<float,V>(dis(rg),dis(rg),dis(rg)),dis(rg));
CHECK(quat<float>::qlen(slerp(q1,q2,0.0f)-q1)<=1e-5);
CHECK(quat<float>::qlen(slerp(q1,q2,1.0f)-q2)<=1e-5);}}
TEST_CASE("slrep_random"){
int x;for(x=1;x<=100000;x++){
quat<float>q1=quat<float>::axis(vec3<float,V>(dis(rg),dis(rg),dis(rg)),dis(rg));
quat<float>q2=quat<float>::axis(vec3<float,V>(dis(rg),dis(rg),dis(rg)),dis(rg));
CHECK(quat<float>::qlen(slerp(q1,q2,dis1(rg)))-float(1)<=1e-5f);}
quat<float>a(1,0,0,0),b(-0.99f,0.1f,0,0),c=slerp(a,b,0.5f);CHECK(c.w>=0.9f);}
TEST_CASE("mat_unit"){
int x;for(x=1;x<=10000;x++){
mat34<float>m=mat34<float>::trs(vec3<float,V>(dis(rg),dis(rg),dis(rg)),quat<float>::axis(vec3<float,V>(dis(rg),dis(rg),dis(rg)),dis(rg)),vec3<float,V>(dis(rg),dis(rg),dis(rg)));
mat34<float>i=mat34<float>::unit();mat34<float>a=m*i;mat34<float>b=i*m;int p,q;for(p=0;p<3;p++){
for(q=0;q<4;q++){CHECK(a.mat[p][q]==m.mat[p][q]);CHECK(b.mat[p][q]==m.mat[p][q]);}}}}
TEST_CASE("mat_associative"){
mat34<float>a=mat34<float>::mv(vec3<float,V>(dis(rg),dis(rg),dis(rg)));
mat34<float>b=mat34<float>::rot(quat<float>::axis(vec3<float,V>(dis(rg),dis(rg),dis(rg)),dis(rg)));
mat34<float>c=mat34<float>::scale(vec3<float,V>(dis(rg),dis(rg),dis(rg)));
mat34<float>m1=(a*b)*c;mat34<float>m2=a*(b*c);CHECK(f(m1,m2));}
TEST_CASE("trs_point"){mat34<float>m=mat34<float>::mv(vec3<float,V>(5,3,-2));
vec3<float,P>p(1,1,1);vec3<float,P>q=m*p;//printf("%f %f f\n",q.x,q.y,q.z);
auto ans=q-vec3<float,P>(6,4,-1);CHECK(len(ans)<=1e-5);}
TEST_CASE("trs_vec"){mat34<float>m=mat34<float>::mv(vec3<float,V>(100,200,300));
vec3<float,V>v(1,0,0);vec3<float,V>w=m*v;CHECK(len(v-w)<=1e-5);}
TEST_CASE("mat_trs"){quat<float>q=quat<float>::axis(vec3<float,V>(0,0,1),1.5707963f);
mat34<float>m=mat34<float>::trs(vec3<float,V>(10,0,0),q,vec3<float,V>(2,1,1));
vec3<float,P>p(1,0,0);auto ans=m*p-vec3<float,P>(10,2,0);CHECK(len(ans)<=1e-5);}
TEST_CASE("inv"){int x,y;mat34<float>m=mat34<float>::trs(vec3<float,V>(dis(rg),dis(rg),dis(rg)),quat<float>::axis(vec3<float,V>(dis(rg),dis(rg),dis(rg)),dis(rg)),vec3<float,V>(2,2,2));
mat34<float>i=mat34<float>::inv(m);
//for(x=0;x<3;x++){for(y=0;y<4;y++)printf("%f ",i.mat[x][y]);printf("\n");}
//CHECK(f(mat34<float>::unit(),i));
mat34<float>i1=mat34<float>::fast_inv(m);CHECK(!f(i,i1));}
TEST_CASE("inv_inv"){mat34<float>m=mat34<float>::trs(vec3<float,V>(10,0,0),quat<float>::axis(vec3<float,V>(dis(rg),dis(rg),dis(rg)),dis(rg)),vec3<float,V>(2,1,1));
mat34<float>m1=mat34<float>::inv(mat34<float>::inv(m));CHECK(f(m,m1));}
TEST_CASE("inv_point"){mat34<float>m=mat34<float>::trs(vec3<float,V>(10,0,0),quat<float>::axis(vec3<float,V>(dis(rg),dis(rg),dis(rg)),dis(rg)),vec3<float,V>(2,1,1));
mat34<float>in=mat34<float>::inv(m);
vec3<float,P>p(dis(rg),dis(rg),dis(rg));vec3<float,P>p1=in*(m*p);CHECK(len(p1-p)<=1e5);}
TEST_CASE("bound"){
bound2<float>b(vec2<float,P>(1,1),vec2<float,P>(3,3));
//printf("%f %f\n",b[0].x,b[0].y);
//printf("%f %f\n",b[1].x,b[1].y);
bound3<float>b1(vec3<float,P>(1,1,1),vec3<float,P>(7,7,7));
//printf("%f %f %f\n",b1[0].x,b1[0].y,b1[0].z);
//printf("%f %f %f\n",b1[1].x,b1[1].y,b1[1].z);
bound3<float>b2(vec3<float,P>(2,3,3),vec3<float,P>(4,4,4));
bound3<float>b3=bound3<float>::intersect(b1,b2);
//std::cerr<<1;
//printf("%f %f %f\n",b3[0].x,b3[0].y,b3[0].z);
//printf("%f %f %f\n",b3[1].x,b3[1].y,b3[1].z);
bound3<float>b4=bound3<float>::add(b1,vec3<float,P>(8,8,8));
//printf("%f %f %f\n",b4[0].x,b4[0].y,b4[0].z);
//printf("%f %f %f\n",b4[1].x,b4[1].y,b4[1].z);
CHECK(bound3<float>::inside(b4,vec3<float,P>(1,2,3)));
//printf("%f\n",b4.area());
}
using std::isnan;
TEST_CASE("pose"){Pose<float>p=Pose<float>::look(vec3<float,P>(0,0,5),vec3<float,P>(0,1,0),vec3<float,V>(0,1,0));
CHECK(dot(p.r,p.u)==0);CHECK(dot(p.r,p.f)==0);CHECK(dot(p.u,p.f)==0);
CHECK(len(p.r)==1);CHECK(len(p.u)==1);CHECK(len(p.f)==1);
Pose<float>p1=Pose<float>::look(vec3<float,P>(0,0,0),vec3<float,P>(0,1,0),vec3<float,V>(0,1,0));
CHECK(!isnan(p1.r.x));CHECK(!isnan(p1.r.y));CHECK(!isnan(p1.r.z));CHECK(len(p.r)==1.0f);}
TEST_CASE("look1"){Pose<float>p=Pose<float>::look(vec3<float,P>(0,0,5),vec3<float,P>(0,1,0),vec3<float,V>(0,1,0));
Pose<float>p1=p;p.trs_fps(2,3,4);CHECK(len(p1.r-p.r)<=1e-5);CHECK(len(p1.u-p.u)<=1e-5);CHECK(len(p1.f-p.f)<=1e-5);}
TEST_CASE("rot_fps"){Pose<float>p=Pose<float>::init();int x;for(x=1;x<=1000;x++){
vec3<float,V>a=nor(vec3<float,V>(dis(rg),dis(rg),dis(rg)));
p.axis(a,dis(rg));if(x%50)p.clean();}CHECK(dot(p.r,p.u)<=1e-5);CHECK(len(p.r)-1.0f<=1e-5);}
TEST_CASE("no_roll"){Pose<float>p=Pose<float>::look(vec3<float,P>(0,0,5),vec3<float,P>(0,1,0),vec3<float,V>(0,1,0));
vec3<float,V>up(0,1,0);int x;for(x=1;x<=50;x++){p.rot_fps(dis(rg),dis(rg),up);}
CHECK(fabs(dot(p.r,up))<=1e-5);}
TEST_CASE("orbit"){Pose<float>p=Pose<float>::look(vec3<float,P>(0,0,5),vec3<float,P>(0,1,0),vec3<float,V>(0,1,0));
vec3<float,P>ctr(0,0,0);float r0=len(p.p-ctr);p.orbit(ctr,vec3<float,V>(0,1,0),1.5f);float r1=len(p.p-ctr);
CHECK(r0==r1);}
TEST_CASE("quat_pose"){Pose<float>p=Pose<float>::look(vec3<float,P>(dis(rg),dis(rg),dis(rg)),vec3<float,P>(dis(rg),dis(rg),dis(rg)),vec3<float,V>(0,1,0));
quat<float>q=p.to_quat();Pose<float>p1=Pose<float>::to_pos(q,p.p);
CHECK(len(p1.p-p.p)<=1e-5);CHECK(len(p1.u-p.u)<=1e-5);CHECK(len(p1.f-p.f)<=1e-5);}
TEST_CASE("rgb"){RGB<float>a(1,1,1);}
TEST_CASE("sample"){
Sampler<Independent<float>,float>in1,in2;int x,n=10;double ex=0,var=0;in1.init(100,200,300,400);in2.init(100,200,300,400);
for(x=1;x<=n;x++){float a=in1.get1d();ex+=a;var+=(double)a*a;}
double a1=ex/n,a2=(var/n)-(a1*a1);//printf("%.6f %.6f\n",a1,a2);
int k;double smx=0,smy=0,smxy=0;for(k=1;k<=n;k++){float v1,v2;in1.get2d(&v1,&v2);
smx+=v1;smy+=v2;smxy+=v1*v2;}double cov=(smxy/n)-(smx/n)*(smy/n);//printf("%.6f\n",cov);
//for(x=1;x<=10;x++)printf("%.6f %.6f\n",in1.get1d(),in2.get1d());
Sampler<Independent<float>,float>c1=in1.clone(100),c2=in1.clone(200);
//printf("c1:%p c2:%p in1:%p\n",c1,c2,in1);
//for(x=1;x<=10;x++)printf("%.6f %.6f\n",c1.get1d(),c2.get1d());
}
//TEST_CASE("filter"){
//Box<float>box(vec2<float,P>(1,2));printf("%f\n",box.integral());
//printf("%f\n",box.evaluate(vec2<float,P>(0,0)));
//}
//TEST_CASE("det 3x3"){
//int x,y;mat<float,3>v;FILE*p=freopen("../a.in","r",stdin);assert(p!=nullptr);for(x=0;x<3;x++){for(y=0;y<3;y++)std::cin>>v.m[x][y];}
//printf("%f\n",v.det());fclose(p);}
//TEST_CASE("det 4x4"){
//int x,y;mat<float,4>v;FILE*p=freopen("../a.in","r",stdin);assert(p!=nullptr);for(x=0;x<4;x++){for(y=0;y<4;y++)std::cin>>v.m[x][y];}
//printf("%f\n",v.det());fclose(p);}
//TEST_CASE("inv 4x4"){
//int x,y;mat<float,4>v;for(x=0;x<4;x++){for(y=0;y<4;y++)v.m[x][y]=dis(rg);}
//mat<float,4>ans=mat<float,4>::inv(v)*v;for(x=0;x<4;x++){for(y=0;y<4;y++)printf("%f ",ans.m[x][y]);printf("\n");}}



