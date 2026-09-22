#pragma once 
#include<cmath>
#include<cstdio>
#include"conf/conf.hpp"
#include"core/ray.hpp"
#include"lens/base.hpp"
#include"camera/shut.hpp"
#include"camera/pose.hpp"
#include"camera/cameraSample.hpp"
#include"film/film.hpp"
namespace veritas{
    //Type
    template<typename T>class Projective;
    template<typename T>class Camera{
    public:
        Camera()=default;
        template<typename C>Camera(C*p):tag(fsytd::idx_of<C,Projective<T>>),ptr(p){static_assert(fsytd::idx_of<C,Projective<T>> >=0,"not a camera type");}
        template<typename F>decltype(auto)visit(F&&f){return fsytd::dispatch<0,F,Projective<T>>(tag,ptr,fsytd::forward_<F>(f)); }
        template<typename F>decltype(auto)visit(F&&f)const{return fsytd::dispatch<0,F,Projective<T>>(tag,ptr,fsytd::forward_<F>(f));}
        void generateRay(ray<T>*r,cameraSample<T>&cs)const{return visit([&](const auto&c){return c.generateRay(r,cs);});}
        Pose<T>&getPos(){return visit([&](auto&c)->Pose<T>&{return c.getPos();});}
        const Pose<T>&getPos()const{return visit([&](const auto&c)->Pose<T>&{return c.getPos();});}        
    private:
        int tag=-1;
        void*ptr=nullptr;
    };
    template<typename T>class Projective{
    public:
        Projective()=default;
        Projective(Pose<T>&pos,Lens<T>&lens,Shut<T>&shut,int w,int h,T fov):pos(pos),lens(lens),shut(shut),w(w),h(h),fov(fov),hf_h(tan(fov/T(2)*veritas::fsytd::PI<T>/T(180.0))),hf_w(T(w)/T(h)*hf_h){};
        void generateRay(ray<T>*r,cameraSample<T>&cs)const{
            T fx=cs.px,fy=cs.py;
            T ndcx=(T(2)*fx)/this->w-T(1),ndcy=T(1)-(T(2)*fy)/this->h;
            T camx=ndcx*hf_w,camy=ndcy*hf_h;
            auto&pos=this->pos;auto&lens=this->lens;
            vec3<T,V>dir=nor(pos.f+pos.r*camx+pos.u*camy);
            lens.build(cs,r,pos,dir);
            r->t=this->shut.sample(cs.t);
        }
        Pose<T>&getPos(){return this->pos;}
        const Pose<T>&getPos()const{return this->pos;}
        Pose<T>pos;
        Lens<T>lens;
        Shut<T>shut;
        int w,h;
        T hf_h,hf_w,fov;
    };
}//namespace veritas
