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
    template<typename T>struct camera_traits;
    template<typename T,typename Filter,typename LensT>class Projective;
    template<typename T,typename Filter,typename LensT>struct camera_traits<Projective<T,Filter,LensT>>{using value_type=T;};
    template<typename tp,typename Filter,typename LensT>class Camera{
    public:
        using T=camera_traits<tp>::value_type;
        Camera(Pose<T>&pos,Lens<LensT>&lens,Shut<T>&shut,Film<Filter>&film,int w,int h):pos(pos),lens(lens),shut(shut),film(&film),w(w),h(h){};
        ~Camera()=default;
        tp&self(){return static_cast<tp&>(this);}
        const tp&self()const{return static_cast<tp&>(this);}
    protected:
        Pose<T>pos;
        Lens<LensT>lens;
        Shut<T>shut;
        Film<Filter>*film;
        int w,h;
    };
    template<typename T,typename Filter,typename LensT>class Projective:public Camera<Projective<T,Filter,LensT>,Filter,LensT>{
    public:
        Projective(Pose<T>&pos,Lens<LensT>&lens,Shut<T>&shut,Film<Filter>&film,int w,int h,T fov):Camera<Projective<T,Filter,LensT>,Filter,LensT>(pos,lens,shut,film,w,h),fov(fov),hf_h(tan(fov/T(2)*veritas::fsytd::PI<T>/T(180.0))),hf_w(T(w)/T(h)*hf_h){};
        void generateRay(ray<T>*r,cameraSample<T>&cs)const{
            T fx=cs.px,fy=cs.py;
            T ndcx=(T(2)*fx)/this->w-T(1),ndcy=T(1)-(T(2)*fy)/this->h;
            T camx=ndcx*hf_w,camy=ndcy*hf_h;
            auto&pos=this->pos;auto&lens=this->lens;
            vec3<T,V>dir=nor(pos.f+pos.r*camx+pos.u*camy);
            lens.build(cs,r,pos,dir);
            r->t=this->shut.sample(cs.t);
        }
        Pose<T>&get_pos(){return this->pos;}
        const Pose<T>&get_pos()const{return this->pos;}
        Film<Filter>*get_film()const{return this->film;}
    protected:
        T hf_h,hf_w,fov;
    };
}//namespace veritas
