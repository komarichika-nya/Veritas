#pragma once
#include<cassert>
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"conf/lim.hpp"
namespace veritas{
    template<typename tp,typename T>class Interaction{
    public:
        vec3<T,P>bary;
        vec3<T,P>pos;
        vec2<T,P>uv;
        vec3<T,V>n;//normal
        vec3<T,V>wo;//the ray's reflect direction
        Interaction()=default;
        Interaction(const vec3<T,P>&pos,const vec3<T,P>&bary,const vec2<T,P>&uv,const vec3<T,V>&n,const vec3<T,V>&wo):pos(pos),bary(bary),uv(uv),n(n),wo(nor(wo)){}
        Interaction(const vec3<T,P>&pos,const vec3<T,P>&bary,const vec3<T,V>&n,const vec3<T,V>&wo):pos(pos),bary(bary),n(n),wo(wo){}
        Interaction(const vec3<T,P>&pos,const vec3<T,P>&bary,const vec2<T,P>&uv):pos(pos),bary(bary),uv(uv){}
        Interaction(const vec3<T,P>&pos,const vec3<T,P>&bary,const vec2<T,P>&uv,const vec3<T,V>&n):pos(pos),bary(bary),uv(uv),n(n){}
        tp&self(){return static_cast<tp&>(*this);}
        const tp&self()const{return static_cast<tp&>(*this);}
    };
    template<typename T>class surface:public Interaction<surface<T>,T>{
    public:
        vec3<T,V>dpu,dpv;
        vec3<T,V>dnu,dnv;
        int face_idx=0;
        vec2<T,P>du,dv;
        bool flip=0;T t=fsytd::lim<T>::max();
        struct{vec3<T,V>n;vec3<T,V>dpu,dpv;vec3<T,V>dnu,dnv;}shading;
        surface()=default;
        surface(const vec3<T,P>&pos,const vec3<T,P>&bary,const vec3<T,V>&n,const vec2<T,P>&uv,const vec3<T,V>&wo,const vec3<T,V>&dpu,const vec3<T,V>&dpv,const vec3<T,V>&dnu,const vec3<T,V>&dnv,const vec2<T,P>&du,const vec2<T,P>&dv,int face_idx,bool flip):
            Interaction<surface<T>,T>(pos,bary,uv,n,nor(wo)),dpu(dpu),dpv(dpv),dnu(dnu),dnv(dnv),du(du),dv(dv),face_idx(face_idx),flip(flip){
            if(flip)this->n=-this->n;shading.n=n;shading.dpu=dpu;shading.dpv=dpv;shading.dnu=dnu,shading.dnv=dnv;
        }
        surface(const vec3<T,P>&pos,const vec3<T,P>&bary,const vec3<T,V>&n,const vec3<T,V>&wo,const vec3<T,V>&dpu,const vec3<T,V>&dpv,const vec3<T,V>&dnu,const vec3<T,V>&dnv,int face_idx,bool flip):
            Interaction<surface<T>,T>(pos,bary,n,wo),dpv(dpv),dpu(dpu),dnu(dnu),dnv(dnv),face_idx(face_idx),flip(flip){
            if(flip)this->n=-this->n;shading.n=n;shading.dpu=dpu;shading.dpv=dpv;shading.dnu=dnu,shading.dnv=dnv;
        }
        surface(const vec3<T,P>&pos,const vec3<T,P>&bary,const vec3<T,V>&n,const vec3<T,V>&wo,T t,int face_idx,bool flip):
            Interaction<surface<T>,T>(pos,bary,n,wo),t(t),face_idx(face_idx),flip(flip){
            if(flip)this->n=-this->n;shading.n=n;shading.dpu=dpu;shading.dpv=dpv;shading.dnu=dnu,shading.dnv=dnv;
        }
        //surface&getSurface(){assert(this->n!=vec3<T,V>(0,0,0));return*surface;}
        //const surface&getSurface()const{assert(this->n!=vec3<T,V>(0,0,0));return*surface;}
                
    };
}//namespace veritas

