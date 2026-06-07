#pragma once
#include<cstddef>
#include<vector>
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"core/bound.hpp"
#include"core/ray.hpp"
#include"surface.hpp"
using std::vector;
template<typename T>class Mesh{
public:
    vector<vec3<T,P>>pos;
    vector<vec3<T,V>>face_normal;
    vector<vec3<T,V>>shading_normal;
    vector<vec2<T,P>>uvs;
    vector<int>face_idx;
    vector<vec3<int,P>>idx;
    int count;
Mesh();
Mesh(const vector<vec3<T,P>>&pos,const vector<vec3<T,V>>&face_normal,const vector<vec3<T,V>>&shading_normal,const vector<vec2<T,P>>&uvs,const vector<vec3<int,P>>&idx,int cnt);
Mesh(const vector<vec3<T,P>>&pos,const vector<vec3<T,V>>&face_normal,const vector<vec3<int,P>>&idx);
Mesh(const vector<vec3<T,P>>&pos,const vector<vec3<int,P>>&idx,int cnt);
Mesh(const vector<vec3<T,P>>&pos,const vector<vec3<int,P>>&idx);
bound3<T>bounds()const;
T intersect(const ray<T>*r,int p)const;
T area(int p)const;
vec3<T,V>get_normal(int p)const;
surface<T>get(const ray<T>*r,int p)const;};
#include"../../src/util/mesh.cpp"

