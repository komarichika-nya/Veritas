#include<iostream>
#include<cstdio>
#include<string>
#include"conf/optional.hpp"
#include"util/mesh.hpp"
#include"util/shape.hpp"
#include"util/surface.hpp"
#include"camera/camera.hpp"
#include"camera/pose.hpp"
#include"sample/base.hpp"
#include"color/rgb.hpp"
#include<random>
#include"core/vec.hpp"
#include<omp.h>
#include<cstdio>
#include"util/mesh.hpp"
#include"util/surface.hpp"
#include"util/fresnel.hpp"
#include<cassert>
#include<vector>
#include"data-struct/kd-tree.hpp"
using namespace veritas;
const int w=800,h=600;
using std::isnan;
veritas::Pose<float>pos=veritas::Pose<float>::init();
DOF<float>lens(20,0.1);
Box<float>f(vec2<float,P>(0.5,0.5));
bound2<int>b(vec2<int,P>(0,0),vec2<int,P>(w-1,h-1));
Film<Box<float>>film(vec2<int,P>(w,h),b);
Shut<float>sh(0,0);
Independent<float>in;
std::vector<int>idx,tri_idx;
std::vector<vec3<float,P>>ps;
std::vector<vec3<float,V>>normal;
Projective<float,Box<float>,DOF<float>>cam(pos,lens,sh,film,w,h,45.0f);
vec3<float,P>sun(0,20,0);vec3<float,V>e1(4,15,2);vec3<float,V>e2(0,15,4);Spectrum<float>sc(10,10,10);
vec3<float,V>sp(const vec3<float,V>&n,Independent<float>&s){
    float u=s.get1d(),v=s.get1d();float phi=2.0*3.1415926535*u;
    float r=sqrt(v);assert(!isnan(r));float x=cos(phi)*r,z=sin(phi)*r,y=sqrt(std::max(0.0f,1.0f-v));vec3<float,V>u1;
    if(abs(n.y)<float(0.9999))u1=nor(cs(vec3<float,V>(0,1.0,0),n));
    else u1=nor(cs(vec3<float,V>(1.0,0,0),n));
    vec3<float,V>v1=cs(n,u1);return x*u1+y*n+z*v1;
}
float eps=veritas::fsytd::lim<float>::eps();
template<typename T>
bool ref(const vec3<T,V>&wi,const vec3<T,V>&n,T eta,vec3<T,V>&wo){
T ci=-dot(wi,n),s2t=eta*eta*(T(1)-ci*ci);if(s2t>T(1))return 0;
T ct=sqrt(std::max(T(0),T(1)-s2t));wo=wi*eta+(eta*ci-ct)*n;return 1;}
template<typename T>
vec3<T,V>mix(const vec3<T,V>&wi,const vec3<T,V>&n,T ro,int dep){vec3<T,V>w=wi+sp(n,in)*ro;return nor(w);}
float hit2(const ray<float>*r){
    //printf("%f %f %f %f\n",r->o.y,r->d.y,-r->o.y/r->d.y,eps);
    //assert(fabs(r->d.y)>eps);
    if(fabs(r->d.y)>eps)return -r->o.y/r->d.y;
    return -1e8f;}
Spectrum<float>render(ray<float>*r,int dep,Mesh<float>&mesh,Independent<float>&local,Kd_tree<float>&kd){
    if(dep>20)return Spectrum<float>(0,0,0);
    bool hit1=0;float mn=1e30f;float h1=1e30f,t=1e30f,h2=hit2(r);int tp=-1;
    surface<float>sur;
    kd.ask(kd.root,r,sur);
    if(sur.t!=fsytd::lim<float>::max())h1=sur.t;
            //printf("h1:%.10f h2:%.10f tp:%.10f\n",h1,h2,tp);
            //exit(0);
           //if(hit1) return Spectrum<float>(sur.n.x*0.5f+0.5f, sur.n.y*0.5f+0.5f, sur.n.z*0.5f+0.5f);;
            if(h1>eps&&h1<t){t=h1;tp=0;}
            if(h2>eps&&h2<t){t=h2;tp=1;}
            if(tp==-1){
                float u=float(0.5)*(r->d.y+float(1.0));
                return Spectrum<float>::lerp(Spectrum<float>(1,1,1),Spectrum<float>(0.3,0.5,0.7),u);
            }  
            vec3<float,P>p=r->o+vec3<float,P>(r->d*t);Spectrum<float>rho;vec3<float,V>n;vec3<float,V>ns;
            if(tp==0){
                printf("1\n");
                n=sur.n;ns=sur.shading.n;
                //printf("ng:%.6f %.6f %.6f ns:%.6f %.6f %.6f\n",n.x,n.y,n.z,ns.x,ns.y,ns.z);
                rho=Spectrum<float>(0.7,0.7,0.7);
            }
            if(tp==1){
                //printf("2\n");
                n=vec3<float,V>(0,1,0);int ck=(int(floor(p.x))+int(floor(p.z)));rho=ck&1?Spectrum<float>(0.2,0.2,0.2):Spectrum<float>(0.8,0.8,0.8);}
            float G=float(1);
            float u=local.get1d(),v=local.get1d();vec3<float,V>rd=u*e1+v*e2;vec3<float,P>pt=sun+vec3<float,P>(rd.x,rd.y,rd.z);
            vec3<float,V>dir=pt-p;vec3<float,V>n1=nor(cs(e1,e2));float s=len(cs(e1,e2));float dis=std::max(len(dir),eps);dir=nor(dir);Spectrum<float>le(0,0,0);
            float s1=dot(dir,n);float s2=dot(dir,n1);if(s1>float(0)&&s2>float(0)){G=s1*s2/(dis*dis);le=rho/float(3.14159265)*G*sc*s;}
            ray<float>r1;r1.o=sur.pos+vec3<float,P>(n*1e-6f);r1.d=sp(n,local);r1.tmn=1e-4f*fsytd::max(1.0f,fsytd::abs(len(sur.pos)));
            if(dot(r1.d,n)*dot(r1.d,ns)<=0.0f)return le;
            Spectrum<float>li=render(&r1,dep+1,mesh,local,kd);
            return rho*li+le;        
}

//Spectrum<float>render1(ray<float>*r,int dep,Mesh<float>&mesh){
    //if(dep>10)return Spectrum<float>(0,0,0);
    //Spectrum<float>li(0,0,0),Spectrum<float>rho(0,0,0);
//    for(int x=0;x<tri_idx.size()/3;x++){
//    auto ck=mesh.intersect(x,r);surface<float>s;
    //if(ck.has()){
      //  s=ck.val();float land=0;
        //vec3<float,P>p=s.pos;
       // vec3<float,V>n=s.n;
        //int t=s.face_idx;
        //bool rev=s.flip;
        //vec3<float,P>po=r->o+r->d*t;
        //if(fsytd::abs(r->d.y)>fsytd::lim<float>::eps())land=-r->o.y/r->d.y;
        //if(land<=0||t<land){return Spectrum<float>(0.8,0.8,0.8);}
        //vec3<float,V>n1=vec3<float,V>(0,1,0);
        //int ck=(int(floor(po.x))+int(floor(po.z)));
        //return ck&1?Spectrum<float>(0.2,0.2,0.2):Spectrum<float>(0.8,0.8,0.8);
    //}
    //}
    //float u=0.5f*r->d.y+1.0f;
    //return Spectrum<float>::lerp(Spectrum<float>(1,1,1),Spectrum<float>(0.3,0.5,0.7),u);
//}
int main(){
int spp=1;
cam.get_pos().mv(vec3<float,P>(0,5,20));
cam.get_pos()=cam.get_pos().look(cam.get_pos().p,vec3<float,P>(0,6,0),vec3<float,V>(0,1,0));
FILE*p=freopen("a.in","r",stdin);
assert(p!=nullptr);
float a,b,c;
int n,n1;std::cin>>n;
int x;for(x=0;x<n;x++)std::cin>>a>>b>>c,ps.push_back({a,b,c}),tri_idx.push_back(x);
int fuc;std::cin>>n1;for(x=0;x<n1;x++)std::cin>>fuc,idx.push_back(fuc);
int nr;std::cin>>nr;for(x=0;x<nr;x++)std::cin>>a>>b>>c,normal.push_back({a,b,c});
vec3<float,P>lo(1e30f,1e30f,1e30f);vec3<float,P>hi(-1e30f,-1e30f,-1e30f);
for(auto&v:ps){lo.x=fsytd::min(lo.x,v.x);lo.y=fsytd::min(lo.y,v.y);lo.z=fsytd::min(lo.z,v.z);
hi.x=fsytd::max(v.x,hi.x);hi.y=fsytd::max(v.y,hi.y);hi.z=fsytd::max(v.z,hi.z);}

printf("bbox (%.4f %.4f %.4f) - (%.4f %.4f %.4f)\n",lo.x,lo.y,lo.z,hi.x,hi.y,hi.z);
float ext=std::max({hi.x-lo.x,hi.y-lo.y,hi.z-lo.z});
float scale=12.0f/ext;
vec3<float,P>c1((hi.x+lo.x)*0.5f,lo.y,(hi.z+lo.z)*0.5f);
for(auto&v:ps){v.x=(v.x-c1.x)*scale;v.y=(v.y-c1.y)*scale;v.z=(v.z-c1.z)*scale;}
fclose(p);
printf("%d %d %d\n",ps.size(),tri_idx.size(),idx.size());// 66 66 96
Shape<Triangle,float>s(0,idx,ps,normal,std::vector<vec3<float,V>>{},std::vector<vec2<float,P>>{});fsytd::allocator<Shape<Triangle,float>>alloc;
//printf("%d %d\n",sizeof(s.idx_vec)/sizeof(int),sizeof(s.pos)/sizeof(vec3<float,P>));
Mesh<float>mesh(&s);
std::vector<int>face;Kd_tree<float>kd(mesh,SplitMode::sah,1.0f,1.0f);
#pragma omp parallel for
for(int y=0;y<h;y++){
Independent<float>local=in.clone(y);
bound2<int>bd(vec2<int,P>(0,y),vec2<int,P>(w-1,y));
auto tile=cam.get_film()->getTile(bd);
    for(int x=0;x<w;x++){
        local.sp(x,y,y*w+x,100);
        int s;Spectrum<float>sum(0,0,0),sumsp(0,0,0);
        for(s=0;s<spp;s++){
        cameraSample<float>cs; 
        cs.px=x+local.get1d();
        cs.py=y+local.get1d();
        local.get2d(&cs.lx,&cs.ly);
        cs.t=local.get1d();
        ray<float>r;
        cam.generateRay(&r,cs);
        //printf("%d\n",mq);
        sum=sum+render(&r,0,mesh,local,kd);
        }
        sum=sum/float(spp);
        tile.addSample(f,vec2<float,P>(x,y),sum,1.0f);
        //tile.addSplat(vec2<T,P>(x,y),sumsp,1.0f);
    }
    film.mergeTile(tile);
}
//double acc=albedo(vec3<float,V>(0,3,0),0.0238f,1000);
//printf("%f\n",acc);
film.writeImage("b.ppm");

//printf("%f\n",(float)cnt/to);
return 0;
}
