#pragma once
#include"core/vec.hpp"
#include"conf/conf.hpp"
#include"conf/lim.hpp"
namespace veritas{
    template<typename T>struct ray{
        vec3<T,P>o;
        vec3<T,V>d;
        T t,tmn;
        mutable T tmx;T time;
        ray():t(0),tmx(fsytd::lim<T>::max()){}
        ray(const vec3<T,P>&o,const vec3<T,V>&d,T t,T tmx,T tmn,T time):o(o),d(d),t(t),tmx(tmx),tmn(tmn),time(time){};
        T operator()(T t){return o+t*d;}    
    };
}//namespace veritas

