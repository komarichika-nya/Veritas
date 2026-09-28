#pragma once
#include"core/mat.hpp"
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"core/ray.hpp"
#include"fsytd/lim.hpp"
#include"fsytd/optional.hpp"
namespace veritas{
    template<typename T>class Transform{
    public:mat34<T>mat,minv;
        Transform():mat(mat34<T>().unit()),minv(mat34<T>().unit()){}
        Transform(const mat34<T>&m):mat(m),minv(m.inv(m)){}
        Transform(const mat34<T>&m,const mat34<T>&minv):mat(m),minv(minv){}
        vec3<T,P>operator()(const vec3<T,P>&p){return mat*p;}
        vec3<T,V>operator()(const vec3<T,V>&p){return mat*p;}
        vec3<T,P>applyInv(const vec3<T,P>&p){return minv*p;}
        vec3<T,V>applyInv(const vec3<T,V>&p){return minv*p;}
    };
}//namespace veritas
