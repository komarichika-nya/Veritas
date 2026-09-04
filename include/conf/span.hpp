#pragma once
#include"conf/lim.hpp"
#include<cassert>
#include<cstddef>
namespace veritas{
namespace fsytd{
    HD inline constexpr size_t dynamic=lim<size_t>::max();
    template<typename T,size_t extent=dynamic>class span{
        HD T*ptr=nullptr;size_t size_=0;
    public:
        HD constexpr span()noexcept=default;
        HD constexpr span(T*first,size_t size):ptr(first),size_(size){
            if constexpr(extent!=dynamic)assert(size==extent&&"error: span range mismatch");
        }
        HD constexpr span(T*first,T*last):ptr(first),size_(static_cast<size_t>(last-first)){
            if constexpr(extent!=dynamic)assert(size_==extent&&"error:span range mismatch");
        }
        template<typename U>
        HD constexpr span(const std::vector<U>&v):ptr(v.data()),size_(v.size()){
            if constexpr(extent!=dynamic)assert(size_==extent&&"error span range mismatch");
        }
        HD constexpr span(const span&other)noexcept=default;
        HD constexpr const T&operator[](size_t x)const{assert(x<size_&&"error:index out of the bound");return ptr[x];}
        HD constexpr const T&front()const{return ptr[0];}
        HD constexpr const T&back()const{return ptr[size_-1];}
        HD constexpr const T*data()const{return ptr;}
        HD constexpr const size_t size()const{if constexpr(extent!=dynamic)return extent;else return size_;}
        HD constexpr const bool empty()const{return size_==0;}
        HD constexpr const T*begin()const noexcept{return ptr;}
        HD constexpr const T*end()const noexcept{return ptr+size_;}
        HD constexpr span<const T,dynamic>sub(size_t l,size_t r)const{assert(l<=r&&r<=size_&&l<=r&&"range error");
            return{ptr+l,r-l};
        }
    };
}//namespace fsytd
}//namespace veritas
