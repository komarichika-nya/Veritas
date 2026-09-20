#pragma once
#include"conf/conf.hpp"
namespace veritas{
namespace fsytd{
    static_assert(sizeof(int)==4,"lim<int> assumes 32-bit");
    static_assert(sizeof(short)==2,"lim<short> assumes 8-bit");
    static_assert(sizeof(long long)==8,"lim<long long> assumes 64-bit");
    static_assert(sizeof(float)==4,"lim<float> assumes IEEE 754");
    static_assert(sizeof(double)==8,"lim<double> assumes IEEE 754");
    #define BITS(x) (sizeof(x)*8)
    #define UMAX(T) static_cast<T>(~static_cast<T>(0))
    #define IMAX(T) static_cast<T>(static_cast<unsigned long long>(UMAX(unsigned long long)>>(64-BITS(T)+1)))
    #define IMIN(T) static_cast<T>(-IMAX(T)-1)

    #define FLT_MAX_VAL 3.40282347e+38f
    #define FLT_MIN_VAL -3.40282347e+38f 
    #define FLT_EPS_VAL 1.19209290e-7f
    #define DBL_MAX_VAL 1.7976931348623157e+308
    #define DBL_MIN_VAL -1.7976931348623157e+308
    //#define DBL_MIN_VAL 2.2250738585072014e-308
    #define DBL_EPS_VAL 2.2204460492503131e-16
    template<typename T>struct lim;
    #define DEF_INT_LIM(T)\
    template<>struct lim<T>{\
        static constexpr T max(){return IMAX(T);}\
        static constexpr T min(){return IMIN(T);}\
    };
    #define DEF_UINT_LIM(T)\
    template<>struct lim<T>{\
        static constexpr T max(){return IMAX(T);}\
        static constexpr T min(){return 0;}\
    };
    DEF_INT_LIM(int);
    DEF_INT_LIM(signed char);
    DEF_INT_LIM(long long);
    DEF_INT_LIM(short);
    DEF_INT_LIM(long);

    DEF_UINT_LIM(unsigned int);
    DEF_UINT_LIM(unsigned char);
    DEF_UINT_LIM(unsigned long);
    DEF_UINT_LIM(unsigned long long);
    DEF_UINT_LIM(unsigned short);
    template<>struct lim<float>{
        static constexpr float max(){return FLT_MAX_VAL;}
        static constexpr float min(){return FLT_MIN_VAL;}
        static constexpr float eps(){return FLT_EPS_VAL;}
    };
    template<>struct lim<double>{
        static constexpr double max(){return DBL_MAX_VAL;}
        static constexpr double min(){return DBL_MIN_VAL;}
        static constexpr double eps(){return DBL_EPS_VAL;}
    };
}//namespace fsystd
}//namespace veritas
