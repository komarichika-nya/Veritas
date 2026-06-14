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
#include"conf/allocator.hpp"
#include"util/transform.hpp"
using std::vector;
template<typename T>struct Buffer{
    const T*ptr=nullptr;size_t sz=0,hash_=0;
    Buffer(const T*ptr,size_t sz):ptr(ptr),sz(sz),hash_(hash(ptr,sz)){}
    bool operator==(const Buffer&b)const{return sz==b.sz&&hash_==b.hash_&&memcmp(ptr,b.ptr,sz*sizeof(T))==0;}
    size_t get_hash(){return hash(ptr,sz);}
};
template<typename T>struct BufferHash{size_t operator()(const Buffer<T>&b)const{return b.hash_;}};
template<typename T>class BufferCache{
    static constexpr int shard=1<<6;
    static std::shared_mutex mutex[shard];
    static std::unordered_set<Buffer<T>,BufferHash<T>>cache[shard];
public:
    static const T*look_or_add(const T*data,size_t n,fsytd::allocator<T>alloc){
        Buffer<T>br(data,n);int s=br.hash_&(shard-1);
        {std::shared_lock lock(mutex[s]);
        if(auto it=cache[s].find(br);it!=cache[s].end())return it->ptr;}
        {std::unique_lock lock(mutex[s]);T*ptr=alloc.allocate(n);
        std::copy(data,data+n,ptr);
        if(auto it=cache[s].find(br);it!=cache[s].end()){alloc.deallocate(ptr,n);return it->ptr;}
        auto[it1,_]=cache[s].insert(Buffer<T>(ptr,n));return it1->ptr;}
    }
};
template<typename T>std::shared_mutex BufferCache<T>::mutex[1<<6];
template<typename T>std::unordered_set<Buffer<T>,BufferHash<T>>BufferCache<T>::cache[1<<6];

template<typename T>
struct alloc{T*ptr=nullptr;size_t n=0;constexpr alloc()=default;constexpr alloc(T*p,size_t n):ptr(p),n(n){};};
template<typename T>class DeviceBuffer{
    static std::unordered_map<Buffer<T>,std::weak_ptr<alloc<T>>,BufferHash<T>>mp;
    static std::mutex m;
public:
    static std::shared_ptr<alloc<T>>upload(const T*h,size_t n){
        const T*p=BufferCache<T>::look_or_add(h,n);
        Buffer<T>key(p,n);std::lock_guard g(m);
        if(auto sp=mp.find(key);sp!=mp.end())if(auto it=sp->second.lock())return it;
        auto it=std::shared_ptr<alloc<T>>(new alloc<T>{nullptr,n},
                [](alloc<T>*a){if(a->ptr){cudaFree(a->ptr);}delete a;});
        cudaMalloc(&it->ptr,n*sizeof(T));
        cudaMemcpy(it->ptr,p,n*sizeof(T),cudaMemcpyHostToDevice);
        mp[key]=it;return it;
    }
};
template<typename T>std::unordered_map<Buffer<T>,std::weak_ptr<alloc<T>>,BufferHash<T>>DeviceBuffer<T>::mp;
template<typename T>std::mutex DeviceBuffer<T>::m;

struct Triangle{};struct Spheres{};
template<typename tp,typename T>class Shape{};
template<typename T>class Shape<Triangle,T>{
    int num_tri,num_vec;
#ifdef __CUDACC__
    std::shared_ptr<fsytd::allocator<T>>idx_vec;
    std::shared_ptr<fsytd::allocator<T>>pos;
    std::shared_ptr<fsytd::allocator<T>>normal;
    std::shared_ptr<fsytd::allocator<T>>tangent;
    std::shared_ptr<fsytd::allocator<T>>uv;
#else
    const int*idx_vec=nullptr;
    const vec3<T,P>*pos=nullptr;
    const vec3<T,V>*normal=nullptr;
    const vec3<T,V>*tangent=nullptr;
    const vec2<T,P>*uv=nullptr;
#endif
    bool is_reverse;
public:
    Shape()=default;
    Shape(bool is_reverse,const vector<int>&idx_vec,const vector<vec3<T,P>>&pos,
    const vector<vec3<T,V>>&normal,const vector<vec3<T,V>>&tangent,const vector<vec2<T,P>>&uv):is_reverse(is_reverse){
#ifdef __CUDACC__
        if(!idx_vec.empty())this->idx_vec=DeviceBuffer<int>::upload(idx_vec.data(),idx_vec.size());
        if(!pos.empty())this->pos=DeviceBuffer<vec3<T,P>>::upload(pos.data(),pos.size());
        if(!normal.empty())this->normal=DeviceBuffer<vec3<T,V>>::upload(normal.data(),normal.size());
        if(!tangent.empty())this->tangent=DeviceBuffer<vec3<T,V>>::upload(tangent.data(),tangent.size());
        if(!uv.empty())this->uv=DeviceBuffer<vec2<T,P>>::upload(uv.data(),uv.size());
#else
        if(!idx_vec.empty())this->idx_vec=BufferCache<int>::look_or_add(idx_vec.data(),idx_vec.size(),fsytd::allocator<int>{});
        if(!pos.empty())this->pos=BufferCache<vec3<T,P>>::look_or_add(pos.data(),pos.size(),fsytd::allocator<vec3<T,P>>{});
        if(!normal.empty())this->normal=BufferCache<vec3<T,V>>::look_or_add(normal.data(),normal.size(),fsytd::allocator<vec3<T,V>>{});
        if(!tangent.empty())this->tangent=BufferCache<vec3<T,V>>::look_or_add(tangent.data(),tangent.size(),fsytd::allocator<vec3<T,V>>{});
        if(!uv.empty())this->uv=BufferCache<vec2<T,P>>::look_or_add(uv.data(),uv.size(),fsytd::allocator<vec2<T,V>>{});
#endif
    }
};
template<typename T>
HD T spherical_area(const vec3<T,V>&a,const vec3<T,V>&b,const vec3<T,V>&c){
    return abs((T(2)*atan2(dot(a,cs(b,c)),T(1)+dot(a,b)+dot(a,c)+dot(b,c))));
}
//scene <- span((host_ptr) or (device_shared_ptr point to device memory(alloc))) <- build() <- shape(): return shared_ptr ifdef CUDACC else host_pointer;
