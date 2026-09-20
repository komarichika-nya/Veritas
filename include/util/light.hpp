#pragma once
#include"fsytd/optional.hpp"
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"core/bound.hpp"
#include"color/rgb.hpp"
#include"util/surface.hpp"
namespace veritas{
    template<typename T>struct liSample{
        Spectrum<T>L;
        vec3<T,V>wi;
        T pdf;
        surface<T>plight;
        liSample()=default;
        liSample(const Spectrum<T>&L,const vec3<T,V>&wi,T pdf,const surface<T>&plight):L(L),wi(wi),pdf(pdf),plight(plight){}
    };
    
    //Light defination
    enum class LightType:uint8_t{Point,Area};
    template<typename T>class PointLight{
    public:
        PointLight()=default;
        PointLight(const Spectrum<T>&I,const Transform<T>&trans,T scale):I(I),trans(trans),scale(scale){}
        Spectrum<T>Phi()const{return T(4)*veritas::fsytd::PI<T>*I;}
        fsytd::optional<liSample<T>>LiSample(const surface<T>&sur,const vec2<T,P>&u){
            vec3<T,P>p=trans(vec3<T,P>(0,0,0));
            vec3<T,V>wi=nor(p-sur.pos);
            Spectrum<T>li=scale*I/fsytd::dist(p,sur.pos);
            return liSample<T>(li,wi,T(1),sur);
        }
        T pdfli(const surface<T>&,vec3<T,V>&)const{return T(0);}
    private:
        Spectrum<T>I;
        Transform<T>trans;
        T scale;
    };
    template<typename T>class Light{
    public:
        Light()=default;
        Light(LightType t,const void*p):type(t),ptr(p){}
        template<typename F>
        decltype(auto)visit(F&&f)const{
            switch(type){
                case LightType::Point:return f(*static_cast<const PointLight<T>*>(ptr));
                //I will add, but not now, trust me.
                //case LightType::Area:return f(*static_cast<const AreaLight<T>*>(ptr));
            }
            __builtin_unreachable();
        }
        Spectrum<T>Phi()const{return visit([&](auto const&l){l.Phi();});}
        fsytd::optional<liSample<T>>LiSample(const surface<T>&sur,vec2<T,P>u)const{
            return visit([&](auto const&l){l.LiSample(sur,u);});
        }
    private:
        LightType type{};
        const void*ptr=nullptr;
    };
    template<LightType tag,typename T>struct Light_traits;
    template<typename T>struct Light_traits<LightType::Point,T>{
        using type=PointLight<T>;
        static constexpr bool is_delta=true;
    };
    //template<typename T>struct Light_traits<LightType::Area>{};
    template<typename T>struct LightScene{
        std::vector<PointLight<T>>points;
        //std::vector<AreaLight<T>>areas;
        //std::vector<Light<T>>handles;
    };
}
