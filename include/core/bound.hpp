#pragma once
#include"conf/conf.hpp"
#include"conf/lim.hpp"
#include"core/vec.hpp"
template<typename T,int N>struct bound_type;
template<typename T>struct bound_type<T,2>{vec2<T,P>mn,mx;};
template<typename T>struct bound_type<T,3>{vec3<T,P>mn,mx;};
template<typename T,int N>
struct bound:bound_type<T,N>{
    using bound_type<T,N>::mx;
    using bound_type<T,N>::mn;
    HD bound();
    HD bound(vec2<T,P>mn,vec2<T,P>mx)requires(N==2);
    HD bound(vec3<T,P>mn,vec3<T,P>mx)requires(N==3);
    HD vec<T,P,N>&operator[](int x);
    HD const vec<T,P,N>&operator[](int x)const;
    HD static bound<T,N>make(const vec<T,P,N>&mn,const vec<T,P,N>&mx);
    HD static bound<T,N>add(const bound<T,N>&b,const vec<T,P,N>&p);
    HD static bound<T,N>intersect(const bound<T,N>&b1,const bound<T,N>&b2);
    HD static bool inside(const bound<T,N>&b,const vec<T,P,N>&v);
    HD bound<T,N>&operator=(const bound<T,N>&b);
    // HD T dis(const bound<T,N>&b,const vec2<T,P>&v)const;
    // HD T dis(const bound<T,N>&b,const vec3<T,P>&v)const;
    HD T area()const;
    HD vec<T,P,N>center()const;
};
template<typename T>
using bound2=bound<T,2>;
template<typename T>
using bound3=bound<T,3>;
#include"../../src/core/bound.cpp"
