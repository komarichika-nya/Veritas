#pragma once 
#include"conf/conf.hpp"
#include"core/ray.hpp"
#include"lens/base.hpp"
#include"lens/dof.hpp"
#include"camera/shut.hpp"
#include"camera/pose.hpp"
#include"camera/cameraSample.hpp"
#include"film/film.hpp"
template<typename tp,typename T,typename LensT,typename FilterT>
class Camera{
public:
    Pose<T>pos;LensT lens;shut<T>sh;Film<FilterT,T>*film;T w,h;
    HD Camera(Pose<T>&pos,LensT&lens,shut<T>&sh,Film<FilterT,T>&film,int w,int h);
    HD void create(const ray<T>*r,cameraSample<T>&sampler)const;
};
template<typename T,typename LensT,typename FilterT>
class Projective:public Camera<Projective<T,LensT,FilterT>,T,LensT,FilterT>{
public:
    T hf_h,hf_w,fov;
    using base=Camera<Projective<T,LensT,FilterT>,T,LensT,FilterT>;
    HD Projective(Pose<T>&pos,LensT&lens,shut<T>&sh,Film<FilterT,T>&film,int w,int h,T fov);
    HD void generateRay(ray<T>*r,cameraSample<T>&cs)const;
};
#include"../../src/camera/camera.cpp"
