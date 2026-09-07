#include"conf/conf.hpp"
namespace veritas{
    namespace fsytd{
    template<typename T,typename deleter>class unique_ptr{
        T*ptr=nullptr;deleter del{}; 
    public:
            HD constexpr unique_ptr()noexcept=default;
            HD explicit unique_ptr(T*p=nullptr)noexcept:ptr(p){}
            HD unique_ptr(T*p,deleter d)noexcept:ptr(p),del(d){}
            HD ~unique_ptr(){if(ptr)del(ptr);}
            HD unique_ptr(const unique_ptr&)=delete;
            HD unique_ptr&operator=(const unique_ptr&)=delete;
            HD unique_ptr(unique_ptr&&p)noexcept:ptr(p.ptr),del(std::move(p.del)){p.ptr=nullptr;}
            HD unique_ptr&operator=(unique_ptr&&p){if(this!=&p){reset(p.release());del=std::move(p.del);}return*this;}
            HD void reset(T*p=nullptr)noexcept{if(ptr!=p){if(ptr)del(ptr);ptr=p;}}
            HD T*get()const{return ptr;}
            HD T&operator*()const{return *ptr;}
            HD T*operator->()const{return ptr;}
            HD T*release(){T*t=ptr;ptr=nullptr;return t;}
            HD explicit operator bool()const{return ptr!=nullptr;} 
        };
    template<typename T,typename deleter>class unique_ptr<T[],deleter>{
        T*ptr=nullptr;deleter del{}; 
    public:
            HD constexpr unique_ptr()noexcept=default;
            HD explicit unique_ptr(T*p=nullptr)noexcept:ptr(p){}
            HD unique_ptr(T*p,deleter d)noexcept:ptr(p),del(d){}
            HD ~unique_ptr(){if(ptr)del(ptr);}
            HD unique_ptr(const unique_ptr&)=delete;
            HD unique_ptr&operator=(const unique_ptr&)=delete;
            HD unique_ptr(unique_ptr&&p)noexcept:ptr(p.ptr),del(std::move(p.del)){p.ptr=nullptr;}
            HD unique_ptr&operator=(unique_ptr&&p){if(this!=&p){reset(p.release());del=std::move(p.del);}return*this;}
            HD void reset(T*p=nullptr)noexcept{if(ptr!=p){if(ptr)del(ptr);ptr=p;}}
            HD T*get()const noexcept{return ptr;}
            HD T&operator[](int x)const noexcept{return ptr[x];} 
            HD T*release(){T*t=ptr;ptr=nullptr;return t;}
        };
    }
}//namespace veritas
