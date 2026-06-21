#include<iostream>
#include<cstdio>
#include<string>
#include"conf/optional.hpp"
#include"util/mesh.hpp"
#include"util/shape.hpp"
#include"util/surface.hpp"
#include"camera/camera.hpp"
#include"camera/pose.hpp"
#include"sample/Independent.hpp"
#include"color/rgb.hpp"
#include<random>
#include"core/vec.hpp"
#include<omp.h>
#include<cstdio>
#include"util/mesh.hpp"
#include"util/surface.hpp"
#include<cassert>
#include<vector>
const int w=800,h=600;
Pose<float>pos=Pose<float>::init();
DOF<float>lens(20,0.1);
Box<float>f(vec2<float,P>(0.5,0.5));
bound2<int>b(vec2<int,P>(0,0),vec2<int,P>(w-1,h-1));
Film<Box<float>,float>film(vec2<int,P>(w,h),b);
shut<float>sh(0,0);
Independent<float>in;
Projective<float,DOF<float>,Box<float>>cam(pos,lens,sh,film,w,h,45.0f);
int main(){
int spp=1;
cam.pos.mv(vec3<float,P>(0,5,20));
cam.pos=cam.pos.look(cam.pos.p,vec3<float,P>(0,6,0),vec3<float,V>(0,1,0));
std::vector<int>idx={2,1,0};
std::vector<vec3<float,P>>ps={{0,6,0},{-3,3,0},{3,3,0}};
Shape<Triangle,float>s(0,idx,ps,std::vector<vec3<float,V>>{},std::vector<vec3<float,V>>{},std::vector<vec2<float,P>>{});fsytd::allocator<Shape<Triangle,float>>alloc;
Mesh<float>mesh;auto m=mesh.build(&s,alloc);
#pragma omp parallel for
for(int y=0;y<h;y++){
Independent<float>local=in.clone(y);
bound2<int>bd(vec2<int,P>(0,y),vec2<int,P>(w-1,y));
auto tile=cam.film->getTile(bd);
    for(int x=0;x<w;x++){
        local.sp(x,y,y*w+h,100);
        int s;Spectrum<float>sum(0,0,0),sumsp(0,0,0);
        for(s=0;s<spp;s++){
        cameraSample<float>cs; 
        cs.px=x+local.get1d();
        cs.py=y+local.get1d();
        local.get2d(&cs.lx,&cs.ly);
        cs.t=local.get1d();
        ray<float>r;
        cam.generateRay(&r,cs);
        Spectrum<float>L(0,0,0);
        if(auto it=fsytd::intersect(&r,ps[0],ps[1],ps[2]);it.has())L=L+Spectrum<float>(1,0,0); 
        sum=sum+L;
        }sum=sum/float(spp);
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
