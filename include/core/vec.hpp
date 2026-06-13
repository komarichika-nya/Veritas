#pragma once
#include<cmath>
#include"conf/conf.hpp"
struct P{};struct V{};
template<typename T,int N>struct vec_type;
template<typename T>struct vec_type<T,2>{T x,y;};
template<typename T>struct vec_type<T,3>{T x,y,z;};
template<typename T>struct vec_type<T,4>{T x,y,z,w;};
template<typename T,typename tag,int N>
struct vec:vec_type<T,N>{
    HD vec();
    HD vec(T x,T y)requires(N==2);
    HD vec(T x,T y,T z)requires(N==3);
    template<typename tg>HD vec(const vec<T,tg,N>&v);
    HD T&operator[](int x);
    HD const T&operator[](int x)const;
    #define VEC(op)\
    template<typename U>\
    HD auto operator op (U s)const{vec<decltype(this->x op s),tag,N>ans;int x;for(x=0;x<N;x++)ans[x]=(&this->x)[x] op s;return ans;}
    VEC(*);VEC(/);
    #undef VEC
    #define VEC_OP(op,tag1,tag2)\
    HD vec<T,tag2,N>operator op (const vec<T,tag1,N>&v)const{vec<T,tag2,N>ans;int x;for(x=0;x<N;x++)ans[x]=(&this->x)[x] op v[x];return ans;}
    VEC_OP(+,tag,tag);VEC_OP(-,V,V);
    #undef VEC_OP
    HD vec<T,tag,N>operator-()const;
    HD vec<T,tag,N>&operator=(const vec<T,tag,N>&v);
    HD bool operator==(const vec<T,tag,N>&a)const;
    HD bool same(const vec<T,tag,N>&a,const vec<T,tag,N>&b)const;
};
template<typename T,int N>
HD vec<T,V,N>operator-(const vec<T,P,N>&v1,const vec<T,P,N>&v2);
template<typename T,typename tag,int N,typename U>
HD vec<T,tag,N>operator*(U s,const vec<T,tag,N>&v);

//useful function layout vec
template<typename T,typename tag,int N>
HD T dot(const vec<T,tag,N>&v1,const vec<T,tag,N>&v2);
template<typename T,typename tag>
HD T cs(const vec<T,tag,2>&v1,const vec<T,tag,2>&v2);
template<typename T,typename tag>
HD vec<T,tag,3>cs(const vec<T,tag,3>&v1,const vec<T,tag,3>&v2);
template<typename T,typename tag,int N>
HD T len(const vec<T,tag,N>&v);
template<typename T,typename tag,int N>
HD vec<T,tag,N>nor(const vec<T,tag,N>&v);
template<typename T,typename U>
HD T lerp(const T&a,const T&b,const U&t);
template<typename T,typename tag,int N>
HD vec<T,tag,N>max(const vec<T,tag,N>&a,const vec<T,tag,N>&b);
template<typename T,typename tag,int N>
HD vec<T,tag,N>min(const vec<T,tag,N>&a,const vec<T,tag,N>&b);
template<typename T,typename tag,int N>
HD vec<T,tag,N>abs(const vec<T,tag,N>&a);
template<typename T,typename tag>
using vec2=vec<T,tag,2>;
template<typename T,typename tag>
using vec3=vec<T,tag,3>;
template<typename T,typename tag>
using vec4=vec<T,tag,4>;
#include"../../src/core/vec.cpp"
