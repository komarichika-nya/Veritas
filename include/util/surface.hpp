#pragma once
#include"conf/conf.hpp"
#include"conf/optional.hpp"
#include"core/vec.hpp"
//basic imformation
template<typename T>
struct shapeIntersection{
    vec3<T,V>pos{0,0,0};
    vec3<T,V>normal{0,0,0};
};

//I will fix, but not now.
template<typename T>
struct surface{
vec3<T,P>pos{0,0,0};vec3<T,V>face_normal{0,0,0};vec3<T,V>shading_normal{0,0,0};T u=lim<T>::max(),v=lim<T>::max();vec3<T,V>dpu{0,0,0},dpv{0,0,0};vec3<T,V>wo{0,0,0};
HD surface();
HD surface(const vec3<T,P>&pos,const vec3<T,V>&face_normal,const vec3<T,V>&shading_normal,T u,T v,const vec3<T,V>&dpu,const vec3<T,V>&dpv,const vec3<T,V>&wo);
HD surface(const vec3<T,P>&pos,const vec3<T,V>&face_normal,const vec3<T,V>&wo);
HD surface(const vec3<T,P>&pos,const vec3<T,V>&shading_normal);};
#include"../../src/util/surface.cpp"


