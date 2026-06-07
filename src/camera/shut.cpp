#include"conf/conf.hpp"
#include"camera/shut.hpp"
template<typename T>
HD shut<T>::shut(T begin,T end):begin(begin),end(end){}
template<typename T>
HD T shut<T>::sample(T u)const{return begin+(end-begin)*u;}
template<typename T>
HD T shut<T>::pdf()const{return T(1)/(end-begin);}
