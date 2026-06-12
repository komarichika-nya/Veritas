#pragma once
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"util/transform.hpp"
struct Triangle{};struct Spheres{};
template<typename tp,typename T>class Shape{};
template<typename T>class Shape<Spheres,T>{
public:T r,zmn,zmx,thetamn,thetamx,phimx;
const Transform*trans;bool is_reverse;};
template<typename T>class<Triangle,T>{
public:int num_tri,num_vec;
const int*idx_vec=nullptr;
const vec3<T,P>*pos=nullptr;
const vec3<T,V>*normal=nullptr;
const vec3<T,V>*tangent=nullptr;
const vec2<T,P>*uv=nullptr;
bool is_reverse;
};
