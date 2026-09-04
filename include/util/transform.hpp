#pragma once
#include"core/mat.hpp"
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"core/ray.hpp"
#include"conf/lim.hpp"
#include"conf/optional.hpp"
namespace veritas{
    template<typename T>class Transform{
    public:mat4<T>m,minv;
        Transform():m(mat4<T>::unit()),minv(mat4<T>::unit()){}
        Transform(const mat4<T>&m):m(mat4<T>::unit()){
            auto pd=mat4<T>::inv(m);if(pd.has())minv=*pd;
            else{int x,y;for(x=0;x<4;x++)for(y=0;y<4;y++)minv[x][y]=veritas::fsytd::lim<float>::max();}
        }
        Transform(const mat<T,4>&m,const mat<T,4>&minv):m(m),minv(minv){}
        Transform mv(const vec3<T,P>&p){
            mat4<T>m=mat4<T>::unit();m[0][3]=p.x;m[1][3]=p.y;m[2][3]=p.z;
            mat4<T>minv=mat4<T>::unit();minv[0][3]=-p.x;minv[1][3]=-p.y;minv[2][3]=-p.z;
            return Transform(m,minv);
        }
        Transform scale(T x,T y,T z){
            mat4<T>m=mat4<T>::unit();m[0][0]=x;m[1][1]=y;m[2][2]=z;
            mat4<T>minv=mat4<T>::unit();minv[0][0]=T(1)/x;minv[1][1]=T(1)/y;minv[2][2]=T(1)/z;
            return Transform(m,minv);
        }
        Transform rotX(T theta){
            T si=sin(fsytd::to_rad(theta)),co=cos(fsytd::to_rad(theta));
            mat4<T>m=mat4<T>::unit();m[1][1]=co;m[1][2]=-si;m[2][1]=si;m[2][2]=co;
            mat4<T>minv=mat4<T>::inv(m);return Transform(m,minv);
        }
        Transform rotY(T theta){
            T si=sin(fsytd::to_rad(theta)),co=cos(fsytd::to_rad(theta));
            mat4<T>m=mat4<T>::unit();m[0][0]=co;m[0][2]=si;m[2][0]=-si;m[2][2]=co;
            mat4<T>minv=mat4<T>::inv(m);return Transform(m,minv);
        }
        Transform rotZ(T theta){
            T si=sin(fsytd::to_rad(theta)),co=cos(fsytd::to_rad(theta));
            mat4<T>m=mat4<T>::unit();m[0][0]=co;m[0][1]=-si;m[1][0]=si;m[1][1]=co;
            return Transform(m,minv);
        }
    };
}//namespace veritas
