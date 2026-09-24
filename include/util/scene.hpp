#pragma once
#include<vector>
#include"util/shape.hpp"
#include"util/light.hpp"
#include"util/mesh.hpp"
namespace veritas{
    template<typename T>struct Scene{
        int w=0,h=0,spp=0;
        Shape<Triangle,T>s;
        Mesh<T>mesh;
        //LightScene<T>light;
        Scene()=default;
        Scene(int w,int h,int spp,const std::vector<vec3<T,P>>&pos,
                const std::vector<int>&face,const std::vector<vec3<T,V>>&normal):w(w),h(h),spp(spp){
            s=Shape<Triangle,T>(0,face,pos,normal,std::vector<vec3<T,V>>{},std::vector<vec2<T,P>>{});
            mesh=Mesh<T>(&s);
        }
    };
}
