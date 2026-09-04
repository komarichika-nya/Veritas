#pragma once
#include<cassert>
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"conf/lim.hpp"
namespace veritas{
    template<typename tp,typename T>class Interaction{
    public:
        vec3<T,P>pos;
        vec2<T,P>uv;
        vec3<T,V>n;//normal
        vec3<T,V>wo;//the ray's reflect direction
        Interaction()=default;
        Interaction(const vec3<T,P>&pos,const vec2<T,P>&uv,const vec3<T,V>&n,const vec3<T,V>&wo):pos(pos),uv(uv),n(n),wo(nor(wo)){}
        Interaction(const vec3<T,P>&pos,const vec3<T,V>&n):pos(pos),n(n){}
        Interaction(const vec3<T,P>&pos,const vec2<T,P>&uv):pos(pos),uv(uv){}
        Interaction(const vec3<T,P>&pos,const vec2<T,P>&uv,const vec3<T,V>&n):pos(pos),uv(uv),n(n){}
        tp&self(){return static_cast<tp&>(this);}
        const tp&self()const{return static_cast<tp&>(this);}
    };
    template<typename T>class surface:Interaction<surface<T>,T>{
        vec3<T,V>dpu,dpv;
        vec3<T,V>dnu,dnv;
        int face_idx=0;
        vec2<T,P>du,dv;
        bool flip=1;
        struct{vec3<T,V>n;vec3<T,V>dpu,dpv;vec3<T,V>dnu,dnv;}shading;
    public:
        surface()=default;
        surface(const vec3<T,P>&pos,const vec3<T,V>&n,vec2<T,P>&uv,vec3<T,V>&wo,vec3<T,V>&dpu,vec3<T,V>&dpv,vec3<T,V>&dnu,vec3<T,V>&dnv,vec2<T,P>&du,vec2<T,P>&dv,int face_idx,bool flip):Interaction<surface<T>,T>(pos,uv,n,nor(wo)),dpu(dpu),dpv(dpv),dnu(dnu),dnv(dnv),du(du),dv(dv),face_idx(face_idx){
            if(flip)n=-n;shading.n=n;shading.dpu=dpu;shading.dpv=dpv;shading.dnu=dnu,shading.dnv=dnv;
        }
        surface(vec3<T,P>&pos,vec3<T,V>&n,vec3<T,V>&dpu,vec3<T,V>&dpv,vec3<T,V>&dnu,vec3<T,V>&dnv,int face_idx,bool flip):Interaction<surface<T>,T>(pos,n),dpv(dpv),dpu(dpu),dnu(dnu),dnv(dnv){
            if(flip)n=-n;shading.n=n;shading.dpu=dpu;shading.dpv=dpv;shading.dnu=dnu,shading.dnv=dnv;
        }
        surface(const vec3<T,P>&pos,const vec3<T,V>&n,int face_idx,bool flip):Interaction<surface<T>,T>(pos,n),face_idx(face_idx),flip(flip){}
        //surface&getSurface(){assert(this->n!=vec3<T,V>(0,0,0));return*surface;}
        //const surface&getSurface()const{assert(this->n!=vec3<T,V>(0,0,0));return*surface;}
                
    };
}//namespace veritas

