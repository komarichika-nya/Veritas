#pragma once//independent
#include<cstdint>
#include"sample/base.hpp"
using u32=uint32_t;
template<typename T>class Independent:public Sampler<Independent<T>,T>{
public:
    u32 st;
    HD void sp(int px,int py,int idx,int dim);
    HD T get1D();
    HD void get2D(T*u,T*v);
    HD void dim(int);
    HD void ref(int);
    HD Independent<T>clone(int idx);
private:
    HD u32 pcg();
};
#include"../../src/sample/Independent.cpp"
