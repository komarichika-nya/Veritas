#include"conf/conf.hpp"
#include"color/rgb.hpp"
#include<cassert>
template<typename T>
HD RGB<T>::RGB()=default;
template<typename T>
HD RGB<T>::RGB(T r,T g,T b){c[0]=r,c[1]=g,c[2]=b;};
template<typename T>
HD T&RGB<T>::operator[](int x){return c[x];}
template<typename T>
HD const T&RGB<T>::operator[](int x)const{return c[x];}
template<typename T>
HD RGB<T>RGB<T>::operator*(T v)const{return{c[0]*v,c[1]*v,c[2]*v};}
template<typename T>
HD RGB<T>RGB<T>::operator/(T v)const{assert(v>0);return{c[0]/v,c[1]/v,c[2]/v};}
template<typename T>
HD bool RGB<T>::is_black()const{return c[0]==T(0)&&c[1]==T(0)&&c[2]==T(0);}
template<typename T>
HD bool RGB<T>::is_white()const{return c[0]==T(1)&&c[1]==T(1)&&c[2]==T(1);}
template<typename T>
HD T RGB<T>::lum()const{return T(0.2126)*c[0]+T(0.7152)*c[1]+T(0.0722)*c[2];}
template<typename T>
HD void RGB<T>::to_srgb(T out[3])const{out[0]=c[0];out[1]=c[1];out[2]=c[2];}
template<typename T>
HD RGB<T>RGB<T>::lerp(const RGB<T>&c1,const RGB<T>&c2,T u){return c1*(T(1)-u)+c2*u;}
template<typename T>
HD RGB<T>RGB<T>::operator+(T v)const{return{c[0]+v,c[1]+v,c[2]+v};}
