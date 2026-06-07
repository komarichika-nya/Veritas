#include"conf/conf.hpp"
#include"lens/dof.hpp"
#include<cstdio>
#include<cassert>
template<typename T>
HD DOF<T>::DOF(T dis,T rds):dis(dis),rds(rds){}
template<typename T>
HD void DOF<T>::build(cameraSample<T>&cs,ray<T>*r,const Pose<T>&pos,const vec3<T,V>&dir)const{
    if(rds>T(0)){
        T dx,dy;sp(&dx,&dy,cs);
        T lx=dx*rds,ly=dy*rds;cs.lx=lx;cs.ly=ly;
        assert(lx*lx+ly*ly<=rds*rds+1e-3f);
        vec3<T,V>pt=pos.p+pos.r*lx+pos.u*ly;
        vec3<T,V>ed=pos.p+dir*(dis/dot(pos.f,dir));
        r->o=pt;r->d=nor(ed-pt);
    }else r->o=pos.p,r->d=dir;
}
template<typename T>
HD void DOF<T>::sp(T*dx,T*dy,cameraSample<T>&cs)const{
    T phi,r;T u=cs.lx*T(2)-T(1);T v=cs.ly*T(2)-T(1);
    if(abs(u)<T(1e-7)&&abs(v)<T(1e-7)){*dx=*dy=T(0);return;}
    const T pi=T(3.14159265358979323846);
    if(abs(u)>abs(v))r=u,phi=pi/T(4)*(v/u);
    else r=v,phi=pi/T(2)-pi/T(4)*(u/v);
    *dx=r*cos(phi);*dy=r*sin(phi);
}
