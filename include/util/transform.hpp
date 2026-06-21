#pragma once
#include"core/mat.hpp"
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"core/ray.hpp"
template<typename T>class Transform{
public:mat<T,4>m,minv;
HD Transform():m(mat<T,4>::unit()),minv(mat<T,4>::unit()){}
HD Transform(const mat<T,4>&m):m(m),minv(mat<T,4>::inv(m)){}
HD Transform(const mat<T,4>&m,const mat<T,4>&minv):m(m),minv(minv){}
HD vec3<T,P>point(const vec3<T,P>&p)const;
HD vec3<T,P>inv_point(const vec3<T,P>&p)const;
HD vec3<T,V>direct(const vec3<T,V>&v)const;
HD vec3<T,V>inv_direct(const vec3<T,V>&v)const;
HD vec3<T,V>normal(const vec3<T,V>&n)const;
HD ray<T>apply(const ray<T>&r)const;
HD Transform operator*(const Transform<T>&t)const{return{m*t.m,minv*t.minv};}
};
//#include"../../src/unil/transform.cpp"
