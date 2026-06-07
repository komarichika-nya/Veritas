#pragma once
#include<new>
#include<cassert>
#include"conf/conf.hpp"
template<typename T>
struct optional{
    alignas(T)char buf[sizeof(T)];
    bool has_val=0;
    optional()=default;
    optional(const T&val):has_val(1){new(buf)T(val);}
    optional(T&&val):has_val(1){new(buf)T(std::forward(val));}
    bool has()const{return has_val;}
    T&val(){assert(has_val);return *reinterpret_cast<T*>(buf);}
    const T&val()const{assert(has_val);return *reinterpret_cast<T*>(buf);}
};
