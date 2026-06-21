#pragma once
#include<cstddef>
#include<vector>
#include<algorithm>
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"core/bound.hpp"
#include"core/ray.hpp"
#include"surface.hpp"
#include"conf/optional.hpp"
#include"conf/lim.hpp"
#include"util/shape.hpp"
#include"conf/span.hpp"
template<typename T>class Mesh{
    inline static std::vector<const Shape<Triangle,T>*>*v=nullptr;
    const Shape<Triangle,T>*getMesh(size_t mesh_idx)const{return (*v)[mesh_idx];}
public:
    constexpr Mesh()=default;
    std::vector<const Shape<Triangle,T>*>*build(const Shape<Triangle,T>*s,fsytd::allocator<Shape<Triangle,T>>alloc);
    HD const T area(size_t mesh_idx,size_t tri_idx)const;
    HD const bound3<T>bounds(size_t mesh_idx,size_t tri_idx)const;
    HD const T solid_angle(size_t mesh_idx,size_t tri_idx,const vec3<T,P>&p)const;

};
template<typename T>struct hit{T t,b0,b1,b2;};
namespace fsytd{
    template<typename T>
    HD const fsytd::optional<hit<T>>intersect(const ray<T>*r,const vec3<T,P>&p0,const vec3<T,P>&p1,const vec3<T,P>&p2){
        vec3<T,V>e1=p1-p0,e2=p2-p0,pv=cs(r->d,e2);T det=dot(e1,pv);
        if(abs(det)<eps)return{};T inv=T(1)/det;
        vec3<T,V>tv=r->o-p0;T u=dot(tv,pv)*inv;if(u<T(0)||u>T(1))return{};
        vec3<T,V>qv=cs(tv,e1);T v=dot(r->d,qv)*inv;if(v<T(0)||u+v>T(1))return{};
        T t=dot(e2,qv)*inv;if(t<r->tmn||t>r->tmx)return{};return hit<T>{t,T(1)-u-v,u,v};
    }
}
#include"../../src/util/mesh.cpp"

