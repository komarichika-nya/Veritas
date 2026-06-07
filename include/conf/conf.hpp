#pragma once
#ifdef __CUDACC__
#define HD __host__ __device__
#else 
#define HD
#endif
constexpr float inf=1e30f;
constexpr float eps=1e-9;
template<typename T>
T abs(T a){return a<0?-a:a;}
