#pragma once
#include<cassert>
#include"core/vec.hpp"
#include"fsytd/lim.hpp"
#include"data-struct/kd-tree.hpp"
#include"core/ray.hpp"
#include"util/surface.hpp"
namespace veritas{
    namespace fsytd{
        //refract function
        template<typename T>vec3<T,V>refract(const vec3<T,V>&i,const vec3<T,V>&n,T eta){
            assert(!std::isnan(eta));T cosi=dot(i,n);
            if(cosi<T(0)){T k=T(1)-eta*eta*(T(1)-cosi*cosi);if(k<T(0))return vec3<T,V>(0,0,0);return i*eta-(eta*cosi+sqrt(max(k,veritas::fsytd::lim<T>::eps())))*n;}
            else{eta=T(1)/eta;assert(!std::isnan(eta));vec3<T,V>nn=-n;cosi=-cosi;T k=T(1)-eta*eta*(T(1)-cosi*cosi);
            if(k<T(0))return vec3<T,V>(0,0,0);return i*eta-(eta*cosi+sqrt(max(k,veritas::fsytd::lim<T>::eps())))*nn;}
        }
        //reflect function
        template<typename T>vec3<T,V>reflect(const vec3<T,V>&wi,const vec3<T,V>&n){return wi-T(2)*dot(wi,n)*n;} 
        //calculate the distance square between two points.
        template<typename T>bool occluded(const ray<T>&r,T tmn,T tmx,Kd_tree<T>&kd){
            surface<T>sur;kd.ask(kd.root,r,sur);return sur.t<tmx&&sur.t>tmn;
        }        
    }//namespace fsytd
}//namespace veritas
