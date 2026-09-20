#pragma once
namespace veritas{
    namespace fsytd{
        template<typename T>constexpr const T PI=3.14159265358979323846;
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
        //swap
        template<typename tp>void swap(tp&a,tp&b){tp x=a;a=b;b=x;}
        //distance
        template<typename T>T dist(const vec3<T,P>&a,const vec3<T,P>&b){
            T x1=a.x-b.x,y1=a.y-b.y,z1=a.z-b.z;
            return x1*x1+y1*y1+z1*z1;
        }
    }//namespace fsytd
}//namespace veritas
