#include<iostream>
#include<cstdio>
#include<string>
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
#include"util/light.hpp"
#include"fsytd/opt.hpp"
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

//sun--light
vec3<float,P>sun(0,20,0);vec3<float,V>e1(4,15,2);vec3<float,V>e2(0,15,4);Spectrum<float>sc(1000,1000,1000);

Spectrum<float>I(50,50,50);Transform<float>trans;
PointLight<float>point(I,&trans,1);
Light<float>light(LightType::Point,&point);

vec3<float,V>sp(const vec3<float,V>&n,Independent<float>&s){
    float u=s.get1d(),v=s.get1d();float phi=2.0*3.1415926535*u;
    float r=sqrt(v);assert(!isnan(r));float x=cos(phi)*r,z=sin(phi)*r,y=sqrt(std::max(0.0f,1.0f-v));vec3<float,V>u1;
    if(abs(n.y)<float(0.9999))u1=nor(cs(vec3<float,V>(0,1.0,0),n));
    else u1=nor(cs(vec3<float,V>(1.0,0,0),n));
    vec3<float,V>v1=cs(n,u1);return x*u1+y*n+z*v1;
}

float eps=veritas::fsytd::lim<float>::eps();

float hit2(const ray<float>*r){if(fabs(r->d.y)>eps)return -r->o.y/r->d.y;return -1e8f;}

Spectrum<float>render(ray<float>*r,int dep,Mesh<float>&mesh,Independent<float>&local,Kd_tree<float>&kd){
    if(dep>20)return Spectrum<float>(0,0,0);
    bool hit1=0;float mn=1e30f;float h1=1e30f,t=1e30f,h2=hit2(r);int tp=-1;
    surface<float>sur;//fsytd::optional<surface<float>>sur1;
    //printf("%p\n",(void*)&sur);
    //kd.ask(kd.root,r,sur);
    for(int x=0;x<idx.size()/3;x++){
        auto ck=mesh.intersect(x,r);
        if(ck.has()){surface<float>sur1=ck.val();if(sur1.t>eps&&sur1.t<mn)mn=sur1.t,sur=sur1,hit1=1;}
    }
    //printf("%p\n",(void*)&sur);
    //printf("%f %f %f\n",sur.pos.x,sur.pos.y,sur.pos.z);
    if(sur.t!=fsytd::lim<float>::max())h1=sur.t;
    if(h1>eps&&h1<t){t=h1;tp=0;}
    if(h2>eps&&h2<t){t=h2;tp=1;}
    Spectrum<float>rho,le(0,0,0);
    //L=L_e+\int_{\Omega^+}f_r(\omega_i,\omega_o,h)L_id\omega.
    if(tp==-1){//ground
        float u=float(0.5)*(r->d.y+float(1.0));
        return rho=Spectrum<float>::lerp(Spectrum<float>(1,1,1),Spectrum<float>(0.3,0.5,0.7),u);
    }  
    vec3<float,P>p=r->o+vec3<float,P>(r->d*t);vec3<float,V>n;vec3<float,V>ns;
    if(tp==0){
        //printf("1\n");
        n=sur.n;ns=sur.shading.n;
        //vec3<float,V>pp=n*0.5f+vec3<float,V>(0.5,0.5,0.5);
        //return Spectrum<float>(pp.x,pp.y,pp.z);
        rho=Spectrum<float>(0.7,0.7,0.7);
    }
    if(tp==1){
        n=vec3<float,V>(0,1,0);int ck=(int(floor(p.x))+int(floor(p.z)));
        return ck&1?Spectrum<float>(0.2,0.2,0.2):Spectrum<float>(0.8,0.8,0.8);
    }
    vec2<float,P>rng(local.get1d(),local.get1d());
    //check visable. spp=1.
    //LiSample
    Spectrum<float>d1(0,0,0),d2(0,0,0);//direct and indirect
    //printf("%p\n",(void*)&sur);
    //printf("%f %f %f\n",sur.pos.x,sur.pos.y,sur.pos.z);
    auto ctx=light.LiSample(sur,rng);
    //printf("%f %f %f\n",ctx.val().plight.pos.x,ctx.val().plight.pos.y,ctx.val().plight.pos.z);
    Spectrum<float>li(0,0,0);float pdf;vec3<float,V>wi;
    if(dot(n,r->d)>0)n=-n;if(dot(ns,r->d)>0)ns=-ns;
    if(ctx.has()){
        li=ctx.val().L;wi=ctx.val().wi;pdf=ctx.val().pdf;
        if(dot(wi,n)>0&&dot(wi,ns)>0){
            ray<float>test;test.o=sur.pos+vec3<float,P>(n.x,n.y,n.z)*test.tmn,test.d=wi;float dis=len(ctx.val().plight.pos-test.o);
            if(!fsytd::occluded(test,test.tmn,dis*(1.0f-1e-4f),kd))d1=li*rho*(1.0f/veritas::fsytd::PI<float>)*dot(test.d,n)/pdf;
            //else return Spectrum<float>(0.5,0,0);
        }//else return Spectrum<float>(0,0,0.5);
    }
    vec3<float,V>dir=nor(sp(n,local));ray<float>nxt;nxt.o=sur.pos+vec3<float,P>(n.x,n.y,n.z)*nxt.tmn,nxt.d=dir;
    d2=render(&nxt,dep+1,mesh,local,kd)*rho;
    return le+d1;//pointlight pdf=1;
}


int main(){
int spp=100;
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
//printf("%d %d %d\n",ps.size(),tri_idx.size(),idx.size());// 66 66 96
Shape<Triangle,float>s(0,idx,ps,normal,std::vector<vec3<float,V>>{},std::vector<vec2<float,P>>{});fsytd::allocator<Shape<Triangle,float>>alloc;
//printf("%d %d\n",sizeof(s.idx_vec)/sizeof(int),sizeof(s.pos)/sizeof(vec3<float,P>));
//printf("%d\n",s.num_vec);
Mesh<float>mesh(&s);
std::vector<int>face;Kd_tree<float>kd(mesh,SplitMode::sah,1.0f,1.0f);
//printf("%d %d\n\n",kd.root,kd.idx.size());
//for(auto x:kd.idx)printf("%d\n",x);
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
film.writeImage("a.ppm");

//printf("%f\n",(float)cnt/to);
return 0;
}
