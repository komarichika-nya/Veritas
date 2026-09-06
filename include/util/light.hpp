#pragma once
#include"core/vec.hpp"
namespace veritas{
    template<typename tp,typename T>class Light{
    public:
        Light()=default;
        tp&self(){return static_cast<tp&>(*this);}
        const tp&self()const{return static_cast<tp&>(*this);}
    };
    template<typename T>class pointLight:public Light{
    public:
        pointLight()=default;
        pointLight(const vec3<T,P>&pos,lightSample<T>&sample);
    protected:
        vec3<T,P>pos;
    };
}
