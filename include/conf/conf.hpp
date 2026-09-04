#pragma once
#include<cstdint>
#include<cstddef>
#include"core/vec.hpp"
#include"conf/lim.hpp"
namespace veritas{
#ifdef __CUDACC__
#define HD __host__ __device__
#else 
#define HD
#endif
    namespace fsytd{
        template<typename T>
        constexpr const T PI=3.14159265358979323846;
        template<typename T>HD T abs(T a){return a<0?-a:a;}
        template<typename T>HD T max(T a,T b){return a>b?a:b;}
        template<typename T>HD T min(T a,T b){return a>b?b:a;}
        template<typename T>HD T to_rad(T deg){return PI<T>/T(180)*deg;}
    
        //hash function
        size_t hash(const void*d,size_t bt){
            const auto*p=static_cast<const unsigned char*>(d);
            size_t h=1469598103934665603ull;
            size_t x;for(x=0;x<bt;x++){h^=size_t(p[x]);h*=1099511628211ull;}
            return h;
        }
        //forward
        template<bool B>struct bool_type{static constexpr bool value=B;};
        using true_type=bool_type<true>;using false_type=bool_type<false>;
        template<typename T>struct remove_reference{using type=T;};
        template<typename T>struct remove_reference<T&>{using type=T;};
        template<typename T>struct remove_reference<T&&>{using type=T;};
        template<typename T>struct is_lvalue_reference:false_type{};
        template<typename T>struct is_lvalue_reference<T&>:true_type{};
        template<typename T>
        constexpr T&&forward_(typename remove_reference<T>::type&tp)noexcept{return static_cast<T&&>(tp);}
        template<typename T>
        constexpr T&&forward_(typename remove_reference<T>::type&&tp)noexcept{
            static_assert(!is_lvalue_reference<T>::value,"cannot forward an rvalue as a lvalue");
            return static_cast<T&&>(tp);
        }
        
        //swap
        template<typename tp>void swap(tp&a,tp&b){tp x=a;a=b;b=x;}
        
        //refract function
        template<typename T>vec3<T,V>refract(const vec3<T,V>&i,const vec3<T,V>&n,T eta){
            assert(!std::isnan(eta));T cosi=dot(i,n);
            if(cosi<T(0)){T k=T(1)-eta*eta*(T(1)-cosi*cosi);if(k<T(0))return vec3<T,V>(0,0,0);return i*eta-(eta*cosi+sqrt(max(k,veritas::fsytd::lim<T>::eps())))*n;}
            else{eta=T(1)/eta;assert(!std::isnan(eta));vec3<T,V>nn=-n;cosi=-cosi;T k=T(1)-eta*eta*(T(1)-cosi*cosi);
            if(k<T(0))return vec3<T,V>(0,0,0);return i*eta-(eta*cosi+sqrt(max(k,veritas::fsytd::lim<T>::eps())))*nn;}
        }

        //reflect function
        template<typename T>vec3<T,V>reflect(const vec3<T,V>&wi,const vec3<T,V>&n){return wi-T(2)*dot(wi,n)*n;} 

    }//namespace fsytd
}//namespace veritas
