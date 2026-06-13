#pragma once
#include<memory>
#include<unordered_set>
#include<cstring>
#include<shared_mutex>
#include<vector>
#include<cuda_runtime.h>
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"util/transform.hpp"
template<typename T>struct Buffer{
    const T*ptr=nullptr;size_t sz=0,hash_=0;
    Buffer(const T*ptr,size_t sz):ptr(ptr),sz(sz),hash_(hash(ptr)){}
    bool operator==(const Buffer&b){return sz==b.sz&&hash_==b.get_hash()&&memcmp(ptr,b.ptr,sz*sizeof(T))==0;}
    size_t get_hash(){return hash(ptr,sz);}
};
template<typename T>struct BufferHash{size_t operator()(const Buffer<T>&b){return b.hash_;}};
template<typename T>class BufferCache{
    static constexpr int shard=1<<6;
    std::shared_mutex mutex[shard];
    std::unordered_set<Buffer<T>,BufferHash<T>>cache[shard];
public:
    const T*look_or_add(const T*data,size_t n){
        Buffer<T>br(data,n);int s=br.hash_&(shard-1);
        {std::shared_lock lock(mutex[s]);
        if(auto it=cache[s].find(br);it!=cache[s].end())return it->ptr;}
        {std::unique_lock lock(mutex[s]);
        if(auto it=cache[s].find(br);it!=cache[s].end())return it->ptr;
        auto[it1,_]=cache[s].insert(br);return it1->ptr;}
    }
};
template<typename T>class DeviceBuffer{
    T*ptr=nullptr;size_t n=0;
public:
    DeviceBuffer()=default;
    DeviceBuffer(const T*h,size_t n):n(n){ptr=cudaMalloc(&ptr,n*sizeof(T));
    cudaMemcpy(ptr,h,n*sizeof(T),cudaMemcpyHostToDevice);}
    ~DeviceBuffer(){if(ptr)cudaFree(ptr);}
    DeviceBuffer(DeviceBuffer&&d)noexcept:ptr(d.ptr),n(d.n){d.ptr=nullptr;}
    DeviceBuffer&operator=(DeviceBuffer&&d)noexcept{if(this!=&d){if(ptr)cudaFree(ptr);
    ptr=d.ptr;n=d.n;d.ptr=nullptr;d.n=0;}return *this;};
    DeviceBuffer operator=(DeviceBuffer&)=delete;
    T*data(){return ptr;}
    const T*data()const{return ptr;}
    size_t size()const{return n;}
};
struct Triangle{};struct Spheres{};
template<typename tp,typename T>class Shape{};
template<typename T>class<Triangle,T>Shape{
public:int num_tri,num_vec;
    const int*idx_vec=nullptr;
    const vec3<T,P>*pos=nullptr;
    const vec3<T,V>*normal=nullptr;
    const vec3<T,V>*tangent=nullptr;
    const vec2<T,P>*uv=nullptr;
    bool is_reverse;
    Shape<Triangle,T>()=default;
    Shape<Triangle,T>(bool is_reverse,const vector<vec3<T,P>>&pos,
    const vector<vec3<T,V>>&normal,const vector<vec3<T,V>>&tangent,const vector<vec2<T,P>>&uv):
    is_reverse(is_reverse),pos(pos),normal(normal),tangent(tangent),uv(uv){}
};
