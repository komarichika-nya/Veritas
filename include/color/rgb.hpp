#pragma once
#include<cassert>
#include"conf/conf.hpp"
namespace veritas{
    template<typename T>
    struct RGB{
        T c[3];
        RGB()=default;
        RGB(T r,T g,T b){c[0]=r,c[1]=g,c[2]=b;};
        T&operator[](int x){return c[x];}
        const T&operator[](int x)const{return c[x];}
        RGB<T>operator*(T v)const{return{c[0]*v,c[1]*v,c[2]*v};}
        RGB<T>operator/(T v)const{assert(v>0);return{c[0]/v,c[1]/v,c[2]/v};}
        bool is_black()const{return c[0]==T(0)&&c[1]==T(0)&&c[2]==T(0);}
        bool is_white()const{return c[0]==T(1)&&c[1]==T(1)&&c[2]==T(1);}
        T lum()const{return T(0.2126)*c[0]+T(0.7152)*c[1]+T(0.0722)*c[2];}
        void to_srgb(T out[3])const{out[0]=c[0];out[1]=c[1];out[2]=c[2];}
        RGB<T>lerp(const RGB<T>&c1,const RGB<T>&c2,T u){return c1*(T(1)-u)+c2*u;}
        RGB<T>operator+(T v)const{return{c[0]+v,c[1]+v,c[2]+v};} 
#define RGB_OP(op)\
    HD RGB operator op (const RGB&a)const{return{c[0] op a.c[0],c[1] op a.c[1],c[2] op a.c[2]};}
    RGB_OP(+);RGB_OP(-);RGB_OP(*);RGB_OP(/)
};
#if defined(RGB_MOD)||!defined(SPECTRUM_MOD)
#define Spectrum RGB
#endif
}//namespace veritas

