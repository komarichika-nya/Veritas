#pragma once
#include<vector>
#include"util/shape.hpp"
#include"util/light.hpp"
#include"util/mesh.hpp"
namespace veritas{
    template<typename T>struct Scene{
        Transform<T>t;
        int w=0,h=0,spp=0;
        Triangle<T>s;
        Mesh<T>mesh;
        //LightScene<T>light;
        Scene()=default;
        Scene(vec3<T,V>&scale,int w,int h,int spp,std::vector<vec3<T,P>>&pos,std::vector<int>&face,std::vector<vec3<T,V>>&normal):w(w),h(h),spp(spp){
            t=Transform<T>(mat34<T>().scale(scale));
            for(auto&p:pos)p=t(p);for(auto&n:normal)n=nor(n);
            s=Triangle<T>(0,face,pos,normal,std::vector<vec3<T,V>>{},std::vector<vec2<T,P>>{});
            mesh=Mesh<T>(&s);
        }
    };
}
