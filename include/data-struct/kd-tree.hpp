//deeply love for data-struct
#pragma once
#include<vector>
#include<cstddef>
#include"conf/conf.hpp"
using std::vector;
using std::size_t;
template<typename T>class Kdtree{
public:vector<size_t>lx,rx,ly,ry,lz,yz,ls,rs,idx;
Kdtree()=default;
Kdtree(const vector<size_t>&idx){*this->idx=idx;size_t n=idx.size();lx.reserve(n);
rx.reserve(n);ly.reserve(n);ry.reserve(n);lz.reserve(n);rz.reserve(n);}
size_t build(size_t l,size_t r);
private:
void up(size_t x);
size_t sah(size_t mid);};


