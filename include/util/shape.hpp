#pragma once 
#include<memory>
#include<unordered_set>
#include<cstring>
#include<shared_mutex>
#include<mutex>
#include<vector>
#include<unordered_map>
//#include<cuda_runtime.h>
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"conf/ptr.hpp"
#include"conf/allocator.hpp"
#include"util/transform.hpp"
namespace veritas{
    using std::vector;
    template<typename T>struct Deleter{size_t n=0;void operator()(T*p)const noexcept{fsytd::allocator<T>{}.deallocate(p,n);}};
    template<typename T>struct Buffer{
        const T*ptr=nullptr;
        size_t sz=0,hash_=0;
        Buffer(const T*p,size_t sz):ptr(p),sz(sz),hash_(fsytd::hash(static_cast<const void*>(p),sz*sizeof(T))){}
        bool operator==(const Buffer&b)const{return sz==b.sz&&hash_==b.hash_&&memcmp(ptr,b.ptr,sz*sizeof(T))==0;}
        size_t get_hash(){return fsytd::hash(static_cast<const void*>(ptr),sz*sizeof(T));}
    };
    template<typename T>struct BufferHash{size_t operator()(const Buffer<T>&b)const{return b.hash_;}};
    template<typename T>class BufferCache{
        static_assert(std::is_trivially_copyable_v<T>);
        static constexpr int shard=1<<6;
        struct Shard{
            std::shared_mutex mutex;
            std::unordered_map<Buffer<T>,fsytd::unique_ptr<T[],Deleter<T>>,BufferHash<T>>cache;
        };
        struct Shards{
            Shard s[shard];
            Shards(){fsytd::allocator<T>{};}
        };
        static Shards&sh(){static Shards s;return s;}
        static fsytd::unique_ptr<T[],Deleter<T>>make(const T*data,size_t n){
            //std::shared_lock lock(std::shared_mutex{});
            fsytd::allocator<T>alloc;
            //printf("p:%p data:%p\n",(void*)p,(void*)data);
            T*p=alloc.allocate(n);
            std::memcpy(p,data,n*sizeof(T));
            //printf("p:%p data:%p\n",(void*)p,(void*)data);
            return fsytd::unique_ptr<T[],Deleter<T>>(p,Deleter<T>{n});
        }
    public:
        static const T*look_or_add(const T*data,size_t n){
            Buffer<T>br(data,n);
            Shard&s=sh().s[br.hash_&(shard-1)];
            {
                std::shared_lock lock(s.mutex);
                if(auto it=s.cache.find(br);it!=s.cache.end())return it->second.get();
            }
            {   fsytd::unique_ptr<T[],Deleter<T>>p=make(data,n);
                std::unique_lock lock(s.mutex);
                if(auto it=s.cache.find(br);it!=s.cache.end())return it->second.get();
                auto[it1,_]=s.cache.emplace(std::move(Buffer<T>(p.get(),n)),std::move(p));
            //printf("it1->ptr:%p\n len(it1->sz):%d\n",it1->ptr,it1->sz);
                return it1->second.get();
            }
        }        
    };

//template<typename T>typename BufferCache<T>::cleaner BufferCache<T>::clean;
//#ifdef __CUDACC__
    //template<typename T>struct alloc{T*ptr=nullptr;size_t n=0;constexpr alloc()=default;constexpr alloc(T*p,size_t n):ptr(p),n(n){};};
    //template<typename T>class DeviceBuffer{
        //static std::unordered_map<Buffer<T>,std::weak_ptr<alloc<T>>,BufferHash<T>>mp;
        //static std::mutex m;
    //public:
        //static std::shared_ptr<alloc<T>>upload(const T*h,size_t n){
            //const T*p=BufferCache<T>::look_or_add(h,n);
            //Buffer<T>key(p,n);std::lock_guard g(m);
            //if(auto sp=mp.find(key);sp!=mp.end())if(auto it=sp->second.lock())return it;
            //auto it=std::shared_ptr<alloc<T>>(new alloc<T>{nullptr,n},[](alloc<T>*a){if(a->ptr){cudaFree(a->ptr);}delete a;});
                //cudaMalloc(&it->ptr,n*sizeof(T));
                //cudaMemcpy(it->ptr,p,n*sizeof(T),cudaMemcpyHostToDevice);
                //mp[key]=it;return it;
        //}
    //};
    //template<typename T>std::unordered_map<Buffer<T>,std::weak_ptr<alloc<T>>,BufferHash<T>>DeviceBuffer<T>::mp;
    //template<typename T>std::mutex DeviceBuffer<T>::m;
//#endif
    struct Triangle{};
    template<typename tp,typename T>class Shape;
    template<typename T>class Shape<Triangle,T>{
    public:
        int num_vec=-1,num_idx=-1;
        const int*idx_vec=nullptr;
        const vec3<T,P>*pos=nullptr;
        const vec3<T,V>*normal=nullptr;
        const vec3<T,V>*tangent=nullptr;
        const vec2<T,P>*uv=nullptr;
        bool is_reverse;
        constexpr Shape()=default;
        constexpr Shape(bool is_reverse,const vector<int>&idx_vec,const vector<vec3<T,P>>&pos,
            const vector<vec3<T,V>>&normal,const vector<vec3<T,V>>&tangent,const vector<vec2<T,P>>&uv):is_reverse(is_reverse),num_vec(pos.size()),num_idx(idx_vec.size()/3){
            if(!idx_vec.empty())this->idx_vec=BufferCache<int>::look_or_add(idx_vec.data(),idx_vec.size());
            //printf("%d %d %d\n",idx_vec[0],idx_vec[1],idx_vec[2]);
            if(!pos.empty())this->pos=BufferCache<vec3<T,P>>::look_or_add(pos.data(),pos.size()); 
            if(!normal.empty())this->normal=BufferCache<vec3<T,V>>::look_or_add(normal.data(),normal.size());
            if(!tangent.empty())this->tangent=BufferCache<vec3<T,V>>::look_or_add(tangent.data(),tangent.size());
            if(!uv.empty())this->uv=BufferCache<vec2<T,P>>::look_or_add(uv.data(),uv.size());
        }
    };
    template<typename T>T spherical_area(const vec3<T,V>&a,const vec3<T,V>&b,const vec3<T,V>&c){
        return fabs((T(2)*atan2(dot(a,cs(b,c)),T(1)+dot(a,b)+dot(a,c)+dot(b,c))));
    }
}//namespace veritas

