#pragma once
#include<cstdint>
#ifdef __CUDACC__
#define HD __host__ __device__
#else 
#define HD
#endif
constexpr float inf=1e30f;
constexpr float eps=1e-6;

//global useful tool function
template<typename T>
HD T abs(T a){return a<0?-a:a;}
template<typename T>
HD size_t hash(const void*d,size_t bt){
const auto*p=static_cast<const unsigned char*>(d);
size_t h=1469598103934665603ull;
size_t x;for(x=0;x<bt;x++){h^=size_t(p[x]);h*=1099511628211ull;}return h;}
