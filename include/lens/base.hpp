#pragma once
#include<cmath>
#include<cassert>
#include"conf/conf.hpp"
#include"camera/cameraSample.hpp"
#include"core/ray.hpp"
#include"camera/pose.hpp"
#include"fsytd/typelist.hpp"
namespace veritas{
    //Type
    template<typename T>class DOF;
    template<typename T>class Lens{
    public:
        Lens()=default;
        template<typename L>Lens(L*p):ptr(p),tag(fsytd::idx_of<L,DOF<T>>){static_assert(fsytd::idx_of<L,DOF<T>> >=0,"not a len type");}
        template<typename F>decltype(auto)visit(F&&f){return fsytd::dispatch<0,F,DOF<T>>(tag,ptr,fsytd::forward_<F>(f));}
        template<typename F>decltype(auto)visit(F&&f)const{return fsytd::dispatch<0,F,DOF<T>>(tag,ptr,fsytd::forward_<F>(f));}
        void build(cameraSample<T>&cs,ray<T>*r,const Pose<T>&pos,const vec3<T,V>&dir)const{return visit([&](auto&l){return l.build(cs,r,pos,dir);});}
    private:
        int tag=-1;
        void*ptr=nullptr;
    };
    template<typename T>class DOF{
    public:
        T dis,rds;
        DOF(T dis,T rds):dis(dis),rds(rds){}
        void build(cameraSample<T>&cs,ray<T>*r,const Pose<T>&pos,const vec3<T,V>&dir)const{
            if(rds>T(0)){T dx,dy;sp(&dx,&dy,cs);T lx=dx*rds,ly=dy*rds;cs.lx=lx;cs.ly=ly;
            assert(lx*lx+ly*ly<=rds*rds+1e-3f);
            vec3<T,P>pt=pos.p+vec3<T,P>(pos.r*lx)+vec3<T,P>(pos.u*ly);
            vec3<T,P>ed=pos.p+vec3<T,P>(dir*(dis/dot(pos.f,dir)));
            r->o=pt;r->d=nor(ed-pt);}else r->o=pos.p,r->d=dir;
        }
        void sp(T*dx,T*dy,cameraSample<T>&cs)const{
            T phi,r;T u=cs.lx*T(2)-T(1);T v=cs.ly*T(2)-T(1);
            if(fsytd::abs(u)<T(1e-7)&&fsytd::abs(v)<T(1e-7)){*dx=*dy=T(0);return;}
            const T pi=T(3.14159265358979323846);
            if(fsytd::abs(u)>fsytd::abs(v))r=u,phi=pi/T(4)*(v/u);
            else r=v,phi=pi/T(2)-pi/T(4)*(u/v);
            *dx=r*std::cos(phi);*dy=r*std::sin(phi);
        }
    };
}//namespace veritas
