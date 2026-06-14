#pragma once
#include"conf/conf.hpp"
template<typename T>
struct RGB{
    T c[3];
    HD RGB();
    HD RGB(T r,T g,T b);
    HD T&operator[](int x);
    HD const T&operator[](int x)const;
#define RGB_OP(op)\
    HD RGB operator op (const RGB&a)const{return{c[0] op a.c[0],c[1] op a.c[1],c[2] op a.c[2]};}
    RGB_OP(+);RGB_OP(-);RGB_OP(*);RGB_OP(/)
    HD RGB operator*(const T v)const;
    HD RGB operator/(const T v)const;
    HD friend RGB operator*(T v,const RGB<T>&rgb){return rgb*v;}
    HD RGB operator+(const T v)const; 
    HD friend RGB operator+(T v,const RGB<T>&rgb){return v+rgb;}
    HD bool is_black()const; 
    HD bool is_white()const;
    HD T lum()const; 
    HD void to_srgb(T out[3])const;
    HD static RGB<T>lerp(const RGB<T>&c1,const RGB<T>&c2,T u);
};
#if defined(RGB_MOD)||!defined(SPECTRUM_MOD)
#define Spectrum RGB
#endif
#include"../../src/color/rgb.cpp"

