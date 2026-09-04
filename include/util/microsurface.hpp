#pragma once
#include<cstdio>
namespace veritas{
    enum struct microMode{GGX,BECKMANN};
    void setMode(microMode&mode){
        if(mode==microMode::GGX)printf("ggx\n");
        else if(mode==microMode::BECKMANN)printf("beckmann\n");
        else printf("Unknow mode %s",mode);
    }
    template<typename T>class MicroDistribution{
    private:
        microMode mode;
        T alpha_u=T(1),alpha_v=T(1),alpha=alpha_u/alpha_v;
        bool sample_visable=T(1);
    public:
        MicroDistribution(microMode&mode,T alpha,bool sample_visable):mode(mode),alpha(alpha),sample_visable(sample_visable){}
        MicroDistribution(microMode&mode,T alpha_u,T alpha_v,bool sample_visable):mode(mode),alpha_u(alpha_u),alpha_v(alpha_v),sample_visable(sample_visable){
            if(alpha_v==T(0)||alpha_u==T(0)throw("alpha_u and alpha_v should not be zero");alpha=alpha_u/alpha_v;
        }
        MicroDistribution type()const{return mode;}
        T get_alpha()const{return alpha;}
        T get_alpha_v()const{return alpha_v;}
        T get_alpha_u()const{return alpha_u;}
        bool check_sample_visable(){return sample_visable;}
        //compute microsurface normal ggx stable.
        vec3<T,V>vndf(const vec3<T,V>&il,const vec3<T,V>&n,T u1,T u2)const{
            vec3<T,V>tt=veritas::fsytd::abs(n.z)>T(0.9999)?nor(cs(vec3<T,V>(1,0,0),n)):nor(cs(vec3<T,V>(0,0,1),n));
            vec3<T,V>bb=cs(tt,n);vec3<T,V>i(dot(il,tt),dot(il,bb),dot(il,n));
            vec3<T,V>vh=nor(vec3<T,V>(i.x*alpha,i.y*alpha,i.z));
            T sq=vh.x*vh.x+vh.y*vh.y;
            vec3<T,V>v1=sq>T(0)?(vec3<T,V>(-vh.y,vh.x,0)/T(sqrt(sq))):vec3<T,V>(1,0,0);
            vec3<T,V>v2=cs(vh,v1);T r=sqrt(u1);
            T phi=T(2)*PI<T>*u2;
            T t1=r*cos(phi),t2=r*sin(phi);
            T s=T(0.5)*(T(1)+vh.z);t2=(T(1)-s)*sqrt(max(1-t1*t1,T(0)))+s*t2;
            vec3<T,V>nh=t1*v1+t2*v2+sqrt(max(T(0),T(1)-t1*t1-t2*t2))*vh;
            vec3<T,V>hl=nor(vec3<T,V>(nh.x*alpha,nh.y*alpha,max(T(0),nh.z)));
            return nor(hl.x*tt+hl.y*bb+hl.z*n);
        }
        T G(const vec3<T,V>&v,const vec3<T,V>&m,const vec3<T,V>&n)const{
            T vm=dot(v,m),vn=dot(v,n);
            if(vm<=T(0)||vn<=T(0))return T(0);
            T b=T(1)+sqrt(1+alpha*alpha*(1-vn*vn)/(vn*vn));
            return T(2)/b;
        }
        //compute GGX distribution
        T D(const vec3<T,V>&n,const vec3<T,V>&m){
            T cosm=dot(n,m);if(cosm<=T(0))return T(0);
            T tanm=(1-cosm*cosm)/(cosm*cosm);
            T x=X(cosm);
            T d=x/(T(PI<T>)*alpha*alpha*cosm*cosm*cosm*cosm)*exp((-tanm)/(alpha*alpha));
            assert(!isnan(d));return d;
        }
        T pdf(const vec3<T,V>&i,const vec3<T,V>&o,const vec3<T,V>&n,const vec3<T,V>&m)const{
            T dv=D(n,m);T g1=G(i,m,n);T g2=G(o,m,n);
            return (veritas::fsytd::abs(dot(i,m))*g1*g2)/(veritas::fsytd::abs(dot(i,n))/veritas::fsytd::abs(dot(m,n)));
        } 
    };
}

