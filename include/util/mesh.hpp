#pragma once
#include<cstddef>
#include<vector>
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"core/bound.hpp"
#include"core/ray.hpp"
#include"surface.hpp"
#include"conf/optional.hpp"
#include<vector>
template<typename T>class Mesh{
    int mesh_idx=-1,tri_idx=-1;
    static vector<const Shape<Triangle,T>*>*all;
    static constexpr T mn_sample=T(3e-4);
    static constexpr T mx_sample=T(6.22);
    const Shape<Triangle,T>*getMesh()const;
public:
    HD Mesh()=default;
    HD Mesh(int mesh_idx,int tri_idx):mesh_idx(mesh_idx),tri_idx(tri_idx){}
    vector<Shape<Triangle,T>>build(const Shape<Triangle,T>*all);
    HD bound3 bounds()const;
};
template<typename T>hit{t,b0,b1,b2};
template<typename T>optional<hit<T>>intersect(const ray<T>*r,T mx,const vec3<T,P>&p0,const vec3<T,P>&p1,const vec3<T,P>&p2);

