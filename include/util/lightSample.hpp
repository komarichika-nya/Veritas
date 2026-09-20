#pragma once
#include"core/vec.hpp"
#include"conf/optional.hpp"
#include"conf/span.hpp"
#include"util/surface.hpp"
namespace veritas{
    template<typename T>struct lightSampleContext{
        vec3<T,P>pos;vec3<T,V>ns,n;
        lightSampleContext()=default;
        lightSampleContext(const surface<T>&s):pos(s.pos),n(s.n),ns(s.shading.n){}
        lightSampleContext(const surface<T>&s):pos(s.pos),n(s.n){}
        lightSampleContext(vec3<T,P>&pos,vec3<T,V>&n,vec3<T,V>&ns):pos(pos),n(n),ns(ns){} 
    };
    enum lightSampleMode{uniform,area};
    template<typename T>struct lightLiSample;
    template<typename T >struct sampledLight{Light<T>light;T p=0};
    template<typename T>class uniformSample{
    public:
        static uniformSample(fsytd::span<Light<T>>light,allocator<T>alloc):light(light.begin(),light.end(),alloc){}
        fsytd::optional<samlpedLight<T>>samlpe(T u){
            if(this->light.empty())return{};
            int idx=fsytd::min(static_cast<int>(u*light.size()),static_cast<int>(light.size()-1));
            return sampledLight<T>{light[idx],T(1)/light.size()};
        }
        T pmf(){return T(1)/this->light.size();}
    private:
        std::vector<Light<T>>light;
    };
}
