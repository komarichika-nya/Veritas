#pragma once
#include<cmath>
#include<cassert>
#include"conf/conf.hpp"
#include"camera/cameraSample.hpp"
#include"core/ray.hpp"
#include"camera/pose.hpp"
namespace veritas{
    template<typename tp>struct lens_traits;
    template<typename T>class DOF;
    template<typename T>struct lens_traits<DOF<T>>{using value_type=T;};
    template<>struct lens_traits<float>{using value_type=float;};
    template<typename tp>class Lens{
    public:
        using T=typename lens_traits<tp>::value_type;
        Lens()=default;
        //Lens(cameraSample<T>&cs,ray<T>*r,const Pose<T>&pos){static_cast<tp*>(this)->build(cs,r,pos);}
        void build(cameraSample<T>&cs,ray<T>*r,const Pose<T>&pos,const vec3<T,V>&dir)const{static_cast<const tp*>(this)->build(cs,r,pos,dir);}
    };
    template<typename T>class DOF:public Lens<DOF<T>>{
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
