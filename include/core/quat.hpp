#pragma once
#include"core/vec.hpp"
#include"core/mat.hpp"
#include"conf/conf.hpp"
template<typename T>struct mat34;
template<typename T>struct quat{T w,x,y,z;
    HD quat();HD quat(T w,T x,T y,T z); 
    HD static quat unit();
    HD static quat norm(const quat<T>&a);
    HD static quat axis(const vec3<T,V>&a,const T ang);
    HD static quat vec(const vec3<T,V>&a,const vec3<T,V>&b);
    template<typename tp>
    HD static vec3<T,tp>rot(const quat<T>&q,const vec3<T,tp>&v);
    HD static quat matr(const mat34<T>&v);
    HD quat conj()const;
    HD quat operator*(const quat<T>&a)const;
    HD quat&operator*=(const quat&v);
    HD quat operator-(const quat&v)const;
    HD bool operator==(const quat&v)const;
    HD quat operator-()const;
    HD static T qlen(const quat<T>&q);
    HD static bool same(const quat<T>&a,const quat<T>&b);
};
template<typename T>HD quat<T>slerp(quat<T>&a,quat<T>&b,T t);
#include"../../src/core/quat.cpp"
