#include"conf/conf.hpp"
#include"sample/Independent.hpp"
#include<cstdio>
template<typename T>HD void Independent<T>::sp(int px,int py,int idx,int dim){st=px^(py<<16)^idx^(dim*0x9e3779b9u);}
template<typename T>HD T Independent<T>::get1D(){return T(pcg())/T(4294967296.0);}
template<typename T>HD void Independent<T>::get2D(T*u,T*v){*u=T(pcg())/T(4294967296.0);*v=T(pcg())/T(4294967296.0);}
template<typename T>HD void Independent<T>::dim(int){};
template<typename T>HD void Independent<T>::ref(int){};
template<typename T>HD u32 Independent<T>::pcg(){u32 la=st;st=st*747796405u+2891336453u;u32 u=(la>>((la>>28u)+4u)^la)*277803737u;u=(u>>22u)^u;return u;}
template<typename T>HD Independent<T>Independent<T>::clone(int idx){Independent<T> cpy=*this;u32 mix=u32(idx)*0x9e3779b9u;cpy.st^=mix;
int x;for(x=1;x<=10;x++)cpy.pcg();return cpy;}
