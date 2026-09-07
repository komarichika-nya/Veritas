#pragma once
#include<cmath>
#include"conf/conf.hpp"
#include"conf/lim.hpp"
namespace veritas{
    struct P{};struct V{};
    template<typename T,int N>struct vec_type;
    template<typename T>struct vec_type<T,2>{T x,y;};
    template<typename T>struct vec_type<T,3>{T x=T(0),y=T(0),z=T(0);};
    template<typename T>struct vec_type<T,4>{T x,y,z,w;};
    template<typename T,typename tag,int N>
    struct vec:vec_type<T,N>{
        vec()=default; 
        vec(T x,T y)requires(N==2):vec_type<T,2>{x,y}{};
        vec(T x,T y,T z)requires(N==3):vec_type<T,3>{x,y,z}{};
        
        template<typename tg>
        explicit vec(const vec<T,tg,N>&v){int x;for(x=0;x<N;x++)(*this)[x]=v[x];}

        T&operator[](int x){return(&this->x)[x];}
        const T&operator[](int x)const{return(&this->x)[x];}
        vec<T,tag,N>operator-()const{int x;vec<T,tag,N>ans;for(x=0;x<N;x++)ans[x]=-(*this)[x];return ans;}
        //vec<T,tag,N>&operator=(const vec<T,tag,N>&v){int x;for(x=0;x<N;x++)(*this)[x]=v[x];return *this;}      
    #define VEC(op)\
        template<typename U>\
        auto operator op (U s)const{vec<decltype(this->x op s),tag,N>ans;int x;for(x=0;x<N;x++)ans[x]=(&this->x)[x] op s;return ans;}
        VEC(*);VEC(/);
    #undef VEC
    #define VEC_OP(op,tag1,tag2)\
        vec<T,tag2,N>operator op (const vec<T,tag1,N>&v)const{vec<T,tag2,N>ans;int x;for(x=0;x<N;x++)ans[x]=(&this->x)[x] op v[x];return ans;}
        VEC_OP(+,tag,tag);VEC_OP(-,V,V);
    #undef VEC_OP
        template<typename tag1>
        requires std::is_same_v<tag,P>&&std::is_same_v<tag1,P>
        vec<T,V,N>operator-(const vec<T,tag1,N>&v)const{vec<T,V,N>ans;int x;for(x=0;x<N;x++)ans[x]=(*this)[x]-v[x];return ans;}
        bool operator==(const vec<T,tag,N>&a)const{bool ck=1;int x;for(x=0;x<N;x++)if((*this)[x]!=a[x])ck=0;return ck;}
    //HD bool same(const vec<T,tag,N>&a,const vec<T,tag,N>&b)const;
    };
    template<typename T,typename tag,int N,typename U>
    vec<T,tag,N>operator*(U s,const vec<T,tag,N>&v){vec<T,tag,N>ans;int x;for(x=0;x<N;x++)ans[x]=v[x]*s;return ans;}
    template<typename T,typename tag,int N>
    T dot(const vec<T,tag,N>&v1,const vec<T,tag,N>&v2){T ans=0;int x;for(x=0;x<N;x++)ans+=v1[x]*v2[x];return ans;}
    template<typename T,typename tag>
    T cs(const vec<T,tag,2>&a,const vec<T,tag,2>&b){return a.x*b.y-a.y*b.x;}
    template<typename T,typename tag>
    vec<T,tag,3>cs(const vec<T,tag,3>&a,const vec<T,tag,3>&b){return{a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
    template<typename T,typename tag,int N>
    T len(const vec<T,tag,N>&v){T ans=0;int x;for(x=0;x<N;x++)ans+=v[x]*v[x];return sqrt(ans);}
    template<typename T,typename tag,int N>
    vec<T,tag,N>nor(const vec<T,tag,N>&v){return v/len(v);}
    template<typename T>
    T lerp(const T&a,const T&b,const T&t){return a*t+b*(1-t);}
    template<typename T,typename tag,int N>
    vec<T,tag,N>max(const vec<T,tag,N>&a,const vec<T,tag,N>&b){vec<T,tag,N>ans;int x;for(x=0;x<N;x++)ans[x]=a[x]>b[x]?a[x]:b[x];return ans;}
    template<typename T,typename tag,int N>
    vec<T,tag,N>min(const vec<T,tag,N>&a,const vec<T,tag,N>&b){vec<T,tag,N>ans;int x;for(x=0;x<N;x++)ans[x]=a[x]>b[x]?b[x]:a[x];return ans;}
    template<typename T,typename tag,int N>
    vec<T,tag,N>abs(const vec<T,tag,N>&a){vec<T,tag,N>ans;int x;for(x=0;x<N;x++)ans[x]=a[x]>0?a[x]:-a[x];return ans;}
    template<typename T,typename tag>
    using vec2=vec<T,tag,2>;
    template<typename T,typename tag>
    using vec3=vec<T,tag,3>;
}//namespace veritas
