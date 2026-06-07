#include"conf/conf.hpp"
struct share{};struct unique{};struct weak{};
template<typename tp,typename typename deleter>class Ptr{};
template<typename T,typename deleter>class Ptr<unique,T,deleter>{
T*ptr;deleter del; 
public:
HD Ptr()=default;
HD explicit Ptr(const T*p=nullptr)noexcept:ptr(p){}
HD Ptr(T*p,deleter d):ptr(p),del(d){}
HD ~Ptr(){del(ptr);}
HD Ptr(const Ptr&&p):ptr(p.ptr){p.ptr=nullptr;}
HD Ptr(const Ptr&&)=delete;
HD Ptr&operator=(const Ptr&&)=delete;
HD Ptr&operator=(const Ptr&&p){if(this!=p)reset(),ptr=p.ptr,p.ptr=nullptr;return *this;}
HD void reset(T*p=nullptr)noexcept{if(ptr)deleter(ptr);}
HD T*get()const{return ptr;}
HD T&operator*()const{return *ptr;}
HD T&operator->()const{return ptr;}
HD T*release(){T*t=ptr;ptr=nullptr;return t;}
HD void reset(T*p=nullptr){if(ptr!=p)delete ptr,ptr=p;}
};
