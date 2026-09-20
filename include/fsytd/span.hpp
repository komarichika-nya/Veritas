#pragma once
#include"fsytd/lim.hpp"
#include<cassert>
#include<cstddef>
namespace veritas{
namespace fsytd{
    inline constexpr size_t dynamic=lim<size_t>::max();
    template<typename T,size_t extent=dynamic>class span{
        T*ptr=nullptr;size_t size_=0;
    public:
        constexpr span()noexcept=default;
        constexpr span(T*first,size_t size):ptr(first),size_(size){
            if constexpr(extent!=dynamic)assert(size==extent&&"error: span range mismatch");
        }
        constexpr span(T*first,T*last):ptr(first),size_(static_cast<size_t>(last-first)){
            if constexpr(extent!=dynamic)assert(size_==extent&&"error:span range mismatch");
        }
        template<typename U>
        constexpr span(const std::vector<U>&v):ptr(v.data()),size_(v.size()){
            if constexpr(extent!=dynamic)assert(size_==extent&&"error span range mismatch");
        }
        constexpr span(const span&other)noexcept=default;
        constexpr const T&operator[](size_t x)const{assert(x<size_&&"error:index out of the bound");return ptr[x];}
        constexpr const T&front()const{return ptr[0];}
        constexpr const T&back()const{return ptr[size_-1];}
        constexpr const T*data()const{return ptr;}
        constexpr const size_t size()const{if constexpr(extent!=dynamic)return extent;else return size_;}
        constexpr const bool empty()const{return size_==0;}
        constexpr const T*begin()const noexcept{return ptr;}
        constexpr const T*end()const noexcept{return ptr+size_;}
        constexpr span<const T,dynamic>sub(size_t l,size_t r)const{assert(l<=r&&r<=size_&&l<=r&&"range error");
            return{ptr+l,r-l};
        }
    };
}//namespace fsytd
}//namespace veritas
