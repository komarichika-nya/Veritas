#pragma once
#include<cstdint>
#include"conf/conf.hpp"
namespace veritas{
    template<typename T>struct sample_traits;
    template<typename T>class Independent;
    template<typename T>struct sample_traits<Independent<T>>{using value_type=T;};
    template<typename tp>class Sampler{
    public:
        using T=typename sample_traits<tp>::value_type;
        Sampler()=default;
        void init(int px,int py,int idx,int dim){static_cast<tp*>(this)->sp(px,py,idx,dim);}
        T get1d(){return static_cast<tp*>(this)->get1D();}
        void get2d(T*u,T*v){static_cast<tp*>(this)->get2D(u,v);}
        void Dim(int n){static_cast<tp*>(this)->dim(n);}
        void bounce(int dep){static_cast<tp*>(this)->ref(dep);}
        tp clone(int idx){return static_cast<tp*>(this)->clone(idx);}
    };
    template<typename T>class Independent:public Sampler<Independent<T>>{
    public:
        using u32=uint32_t;
        u32 st;
        //xor-combining these lets seeds collide: idx=py*w+px makes px^idx depend
        //only on px>>5 when w%32==0, so 32-pixel spans shared one sequence.
        void sp(int px,int py,int idx,int dim){
            u32 h=u32(px)*0x9e3779b9u;
            h=(h^u32(py))*0x85ebca6bu;h^=h>>15;
            h=(h^u32(idx))*0xc2b2ae35u;h^=h>>13;
            h=(h^u32(dim))*0x27d4eb2fu;h^=h>>16;
            st=h?h:0x9e3779b9u;
        }
        T get1D(){return T(pcg())/T(4294967296.0);}
        void get2D(T*u,T*v){*u=T(pcg())/T(4294967296.0);*v=T(pcg())/T(4294967296.0);}
        void dim(int){};
        void ref(int){};
        u32 pcg(){u32 la=st;st=st*747796405u+2891336453u;u32 u=(la>>((la>>28u)+4u)^la)*277803737u;u=(u>>22u)^u;return u;}
        Independent<T>clone(int idx){
            Independent<T> cpy=*this;u32 mix=u32(idx)*0x9e3779b9u;cpy.st^=mix;
            int x;for(x=1;x<=10;x++)cpy.pcg();return cpy;
        } 
    }; 
}//namespace veritas
