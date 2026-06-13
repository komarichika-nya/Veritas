#pragma once
#include"core/vec.hpp"
#include"conf/conf.hpp"
template<typename T>struct ray{
    vec3<T,P>o;vec3<T,V>d;T t,tmn;mutable T tmx;T time;
    HD ray();
    HD ray(const vec3<T,P>&o,const vec3<T,V>&d,T t,T mx,T mn,T time);
    HD T operator()(T t);
};
#include"../../src/core/ray.cpp"
