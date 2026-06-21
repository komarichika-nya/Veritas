//deeply love for data-struct
#pragma once
#include<vector>
#include<cstddef>
#include"conf/conf.hpp"
#include"conf/span.hpp"
#include"util/shape.hpp"
template<typename T>class Kdtree{
//allocator a mem -> idx.
public:
    constexpr Kdtree(const fsytd::span<Shape<Triangle,T>>&sp){}
    static constexpr build(size_t l,size_t r){if(l>r)return 0;size_t mid=sah(l,r);up(mid);return mid;}
};


