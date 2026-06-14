#include"conf/conf.hpp"
template<typename tp,typename T>class Sampler{
public:
    HD Sampler()=default;
    HD void init(int px,int py,int idx,int dim){static_cast<tp*>(this)->sp(px,py,idx,dim);}
    HD T get1d(){return static_cast<tp*>(this)->get1D();}
    HD void get2d(T*u,T*v){static_cast<tp*>(this)->get2D(u,v);}
    HD void Dim(int n){static_cast<tp*>(this)->dim(n);}
    HD void bounce(int dep){static_cast<tp*>(this)->ref(dep);}
    HD tp clone(int idx){return static_cast<tp*>(this)->clone(idx);}
};
