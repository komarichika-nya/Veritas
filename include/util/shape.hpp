#pragma once
#include<memory>
#include<unordered_set>
#include<cstring>
#include<shared_mutex>
#include<mutex>
#include<vector>
#include<unordered_map>
#include<cuda_runtime.h>
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"conf/allocotor.hpp"
#include"util/transform.hpp"
using std::vector;
using fsytd::allocotor;
template<typename T>struct Buffer{
    const T*ptr=nullptr;size_t sz=0,hash_=0;
    Buffer(const T*ptr,size_t sz):ptr(ptr),sz(sz),hash_(hash(ptr,sz)){}
    bool operator==(const Buffer&b)const{return sz==b.sz&&hash_==b.hash_&&memcmp(ptr,b.ptr,sz*sizeof(T))==0;}
    size_t get_hash(){return hash(ptr,sz);}
};
template<typename T>struct BufferHash{size_t operator()(const Buffer<T>&b)const{return b.hash_;}};
template<typename T>class BufferCache{
    static constexpr int shard=1<<6;
    std::shared_mutex mutex[shard];
    std::unordered_set<Buffer<T>,BufferHash<T>>cache[shard];
public:
    const T*look_or_add(const T*data,size_t n,allocotor<T>alloc){
        Buffer<T>br(data,n);int s=br.hash_&(shard-1);
        {std::shared_lock lock(mutex[s]);
        if(auto it=cache[s].find(br);it!=cache[s].end())return it->ptr;}
        {std::unique_lock lock(mutex[s]);T*ptr=alloc.allocate(n*sizof(T));
        std::copy(data,data+n,ptr);
        if(auto it=cache[s].find(br);it!=cache[s].end()){alloc.deallocote(ptr,n*sizeof(T));return it->ptr;}
        auto[it1,_]=cache[s].insert(br);return it1->ptr;}
    }
};
template<typename T>class DeviceBuffer{
    struct alloc{T*ptr=nullptr;size_t n=0;};
    std::unordered_map<Buffer<T>,std::weak_ptr<alloc>,BufferHash<T>>mp;
    std::mutex m;
public:
    std::shared_ptr<alloc>upload(const T*h,size_t n){
        Buffer<T>key(h,n);std::lock_guard g(m);
        if(auto sp=mp.find(key);sp!=mp.end())if(auto it=sp->second.lock())return it;
        auto it=std::shared_ptr<alloc>(new alloc{nullptr,n},[](alloc*a){if(a->ptr){cudaFree(a->ptr);}delete a;});
        cudaMalloc(&it->ptr,n*sizeof(T));
        cudaMemcpy(it->ptr,h,n*sizeof(T),cudaMemcpyHostToDevice);
        mp[key]=it;return it;
    }
};
struct Triangle{};struct Spheres{};
template<typename tp,typename T>class Shape{};
template<typename T>class Shape<Triangle,T>{
public:int num_tri,num_vec;
    const int*idx_vec=nullptr;
    const vec3<T,P>*pos=nullptr;
    const vec3<T,V>*normal=nullptr;
    const vec3<T,V>*tangent=nullptr;
    const vec2<T,P>*uv=nullptr;
    bool is_reverse;
    Shape()=default;
    Shape(bool is_reverse,const vector<vec3<T,P>>&pos,
    const vector<vec3<T,V>>&normal,const vector<vec3<T,V>>&tangent,const vector<vec2<T,P>>&uv):
    is_reverse(is_reverse),pos(pos.data()),normal(normal.data()),tangent(tangent.data()),uv(uv.data()){}
};
template<typename T>
T spherical_area(const vec3<T,V>&a,const vec3<T,V>&b,const vec3<T,V>&c){
    return abs((T(2)*atan2(dot(a,cs(b,c))))/(T(1)+dot(a,b)+dot(a,c)+dot(b,c)));
}

