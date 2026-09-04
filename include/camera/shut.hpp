#pragma once
#include"conf/conf.hpp"
 namespace veritas{
    template<typename T>struct Shut{
        T begin,end;
        Shut(T begin,T end):begin(begin),end(end){}
        T sample(T u)const{return begin+(end-begin)*u;}
        T pdf()const{return T(1)/(end-begin);}
    };
}//namespace veritas


