#pragma once
#include"conf/conf.hpp"
namespace veritas{
template<typename T>
    struct cameraSample{
        T px,py,lx,ly,t;
        cameraSample():px(0),py(0),lx(0),ly(0),t(0){};
    };
}//namespace veritas
