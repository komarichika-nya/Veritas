#pragma once
#include<cstdint>
#include"fsytd/typelist.hpp"
namespace veritas{
    template<typename T>class Independent;
    template<typename T>class Sampler{
    public:
        Sampler()=default;
        template<typename S>Sampler(S*p):tag(fsytd::idx_of<S,Independent<T>>),ptr(p){static_assert(fsytd::idx_of<S,Independent<T>> >=0,"not a Sampler type");}
        template<typename F>decltype(auto)visit(F&&f){return fsytd::dispatch<0,F,Independent<T>>(tag,ptr,fsytd::forward_<F>(f));}
        template<typename F>decltype(auto)visit(F&&f)const{return fsytd::dispatch<0,F,Independent<T>>(tag,ptr,fsytd::forward_<F>(f));}
        void sp(int px,int py,int idx,int dim){return visit([&](auto&s){return s.sp(px,py,idx,dim);});}
        T get1d(){return visit([&](auto&s){return s.get1d();});}
        void get2d(T*u,T*v){return visit([&](auto&s){return s.get2d(u,v);});}
        Sampler<T>clone(int idx){return visit([&](auto&s){return Sampler<T>(new auto(s.clone(idx)));});}
        ~Sampler(){if(ptr)visit([&](auto&s){delete&s;});ptr=nullptr;}
        Sampler(const Sampler&)=delete;
        Sampler&operator=(const Sampler&)=delete;
        Sampler(Sampler&s)noexcept:ptr(s.ptr),tag(s.tag){s.ptr=nullptr;s.tag=-1;}
        Sampler&operator=(Sampler&s)noexcept{if(this!=&s){if(ptr)visit([&](auto&sa){delete&sa;});
        ptr=s.ptr;tag=s.tag;s.ptr=nullptr;s.tag=-1;}return*this;}
    private:
        void*ptr=nullptr;
        int tag=-1;
    };
    template<typename T>class Independent{
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
        T get1d(){return T(pcg())/T(4294967296.0);}
        void get2d(T*u,T*v){*u=T(pcg())/T(4294967296.0);*v=T(pcg())/T(4294967296.0);}
        u32 pcg(){u32 la=st;st=st*747796405u+2891336453u;u32 u=(la>>((la>>28u)+4u)^la)*277803737u;u=(u>>22u)^u;return u;}
        u32 xorshift(){u32 x=st;x^=x<<13;x^=x>>17;x^=x<<5;st=x;return x;}
        Independent<T>clone(int idx){
            Independent<T> cpy=*this;u32 mix=u32(idx)*0x9e3779b9u;cpy.st^=mix;
            int x;for(x=1;x<=10;x++)cpy.pcg();return cpy;
        } 
    }; 
}//namespace veritas
