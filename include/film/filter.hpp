#pragma once
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"film/filterSample.hpp"
namespace veritas{
    template<typename T>class Box{
    public:
        using value_type=T;
        Box()=default;
        Box(const vec2<T,P>&r):r(r){}
        vec2<T,P>radius()const{return r;}
        T integral()const{return T(4)*r.x*r.y;}
        T evaluate(const vec2<T,P>&p)const{return fsytd::abs(p.x)<=r.x&&fsytd::abs(p.y)<=r.y;}
        filterSample<T>sample(const vec2<T,P>&v)const{return{{lerp(-r.x,r.x,v.x),lerp(-r.y,r.y,v.y)},T(1)};}
    private:vec2<T,P>r;
    };
}//namespace veritas
