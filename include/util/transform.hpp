#pragma once
#include"core/mat.hpp"
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"core/ray.hpp"
#include"fsytd/lim.hpp"
#include"fsytd/optional.hpp"
namespace veritas{
    template<typename T>class Transform{
    public:mat4<T>mat,minv;
        Transform():mat(mat4<T>().unit()),minv(mat4<T>().unit()){}
        Transform(const mat4<T>&m):mat(mat4<T>().unit()){
            auto pd=mat4<T>().inv(mat);if(pd.has())minv=*pd;
            else{int x,y;for(x=0;x<4;x++)for(y=0;y<4;y++)minv.m[x][y]=veritas::fsytd::lim<float>::max();}
        }
        Transform(const mat4<T>&m,const mat4<T>&minv):mat(m),minv(minv){}
        vec3<T,P>operator()(const vec3<T,P>&p)const{
            vec<T,P,4>p4(p.x,p.y,p.z,T(1));vec<T,P,4>res=mat*p4;
            return vec3<T,P>(res.x,res.y,res.z);
        }
        vec3<T,P>applyInv(const vec3<T,P>&p)const{
            vec<T,P,4>p4(p.x,p.y,p.z,T(1));vec<T,P,4>res=minv*p4;
            return vec3<T,P>(res.x,res.y,res.z);
        }
    };
}//namespace veritas
