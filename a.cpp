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
#include<cassert>
#include<vector>
using namespace veritas;
const int w=800,h=600;

veritas::Pose<float>pos=veritas::Pose<float>::init();
DOF<float>lens(20,0.1);
Box<float>f(vec2<float,P>(0.5,0.5));
bound2<int>b(vec2<int,P>(0,0),vec2<int,P>(w-1,h-1));
Film<Box<float>>film(vec2<int,P>(w,h),b);
Shut<float>sh(0,0);
Independent<float>in;
Projective<float,Box<float>,DOF<float>>cam(pos,lens,sh,film,w,h,45.0f);
Spectrum<float>render(ray<float>*r,surface<float>&s){return Spectrum<float>(1,1,1);}
int main(){
int spp=1;
cam.get_pos().mv(vec3<float,P>(0,5,20));
cam.get_pos()=cam.get_pos().look(cam.get_pos().p,vec3<float,P>(0,6,0),vec3<float,V>(0,1,0));
FILE*p=freopen("a.out","r",stdin);
assert(p!=nullptr);
std::vector<int>idx;
std::vector<vec3<float,P>>ps;float a,b,c;
int n,n1;std::cin>>n>>n1;
int x;for(x=1;x<=n;x++)std::cin>>a>>b>>c,ps.push_back({a,b,c});
int fuck;for(x=1;x<=n1;x++)std::cin>>fuck,idx.push_back(fuck);
fclose(p);
Shape<Triangle,float>s(0,idx,ps,std::vector<vec3<float,V>>{},std::vector<vec3<float,V>>{},std::vector<vec2<float,P>>{});fsytd::allocator<Shape<Triangle,float>>alloc;
printf("%d %d\n",sizeof(s.idx_vec)/sizeof(int),sizeof(s.pos)/sizeof(vec3<float,P>));
Mesh<float>mesh(&s);
#pragma omp parallel for
for(int y=0;y<h;y++){
Independent<float>local=in.clone(y);
bound2<int>bd(vec2<int,P>(0,y),vec2<int,P>(w-1,y));
auto tile=cam.get_film()->getTile(bd);
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
        for(int mq=0;mq<10;mq++){
        //ask bvh is hit ?
        //printf("%d\n",mq);
        auto ck=mesh.intersect(mq,&r); 
        if(ck.has()){sum=sum+render(&r,ck.val());break;}
        }
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
BufferCache<int>::clean();
BufferCache<vec3<float,P>>::clean();
return 0;
}
