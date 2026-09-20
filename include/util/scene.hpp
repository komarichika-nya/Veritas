#pragma once
#include"util/light.hpp"
#include"util/mesh.hpp"
namespace veritas{
    template<typename T>struct Scene{
        std::vector<Mesh<T>>mesh;
        LightScene<T>light;
    };
}
