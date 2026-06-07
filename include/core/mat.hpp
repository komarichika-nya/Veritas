#pragma once
#include"core/vec.hpp"
#include"core/quat.hpp"
#include"conf/conf.hpp"
template<typename T>struct quat;
template<typename T>struct mat34{T mat[3][4];
HD mat34();
HD static mat34<T>unit();
HD static mat34<T>mv(const vec3<T,V>&v);
HD static mat34<T>scale(const vec3<T,V>&v);
HD static mat34<T>rot(const quat<T>&b);
HD static mat34 bas(const vec3<T,V>&x,const vec3<T,V>&y,const vec3<T,V>&z,const vec3<T,V>&wd);
HD static mat34<T>trs(const vec3<T,V>&t,const quat<T>&r,const vec3<T,V>&s);
HD inline mat34<T>operator*(const mat34<T>&v)const;
HD inline mat34<T>&operator*=(const mat34<T>&v);
HD inline vec3<T,P>operator*(const vec3<T,P>&v)const;
HD inline vec3<T,V>operator*(const vec3<T,V>&v)const;
HD static mat34<T>inv(const mat34<T>&out);
HD static void decompose(const mat34<T>&v,vec3<T,V>&t,quat<T>&q,vec3<T,V>&s);
HD static mat34<T>fast_inv(const mat34<T>&out);
HD static T det(const mat34<T>&out);
HD void to_3x3(T(&v)[3][3])const;};
#include"../../src/core/mat.cpp"
