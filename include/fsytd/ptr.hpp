#include"conf/conf.hpp"
namespace veritas{
    namespace fsytd{
    template<typename T,typename deleter>class unique_ptr{
        T*ptr=nullptr;deleter del{}; 
    public:
             constexpr unique_ptr()noexcept=default;
             explicit unique_ptr(T*p=nullptr)noexcept:ptr(p){}
             unique_ptr(T*p,deleter d)noexcept:ptr(p),del(d){}
             ~unique_ptr(){if(ptr)del(ptr);}
             unique_ptr(const unique_ptr&)=delete;
             unique_ptr&operator=(const unique_ptr&)=delete;
             unique_ptr(unique_ptr&&p)noexcept:ptr(p.ptr),del(std::move(p.del)){p.ptr=nullptr;}
             unique_ptr&operator=(unique_ptr&&p){if(this!=&p){reset(p.release());del=std::move(p.del);}return*this;}
             void reset(T*p=nullptr)noexcept{if(ptr!=p){if(ptr)del(ptr);ptr=p;}}
             T*get()const{return ptr;}
             T&operator*()const{return *ptr;}
             T*operator->()const{return ptr;}
             T*release(){T*t=ptr;ptr=nullptr;return t;}
             explicit operator bool()const{return ptr!=nullptr;} 
        };
    template<typename T,typename deleter>class unique_ptr<T[],deleter>{
        T*ptr=nullptr;deleter del{}; 
    public:
             constexpr unique_ptr()noexcept=default;
             explicit unique_ptr(T*p=nullptr)noexcept:ptr(p){}
             unique_ptr(T*p,deleter d)noexcept:ptr(p),del(d){}
             ~unique_ptr(){if(ptr)del(ptr);}
             unique_ptr(const unique_ptr&)=delete;
             unique_ptr&operator=(const unique_ptr&)=delete;
             unique_ptr(unique_ptr&&p)noexcept:ptr(p.ptr),del(std::move(p.del)){p.ptr=nullptr;}
             unique_ptr&operator=(unique_ptr&&p){if(this!=&p){reset(p.release());del=std::move(p.del);}return*this;}
             void reset(T*p=nullptr)noexcept{if(ptr!=p){if(ptr)del(ptr);ptr=p;}}
             T*get()const noexcept{return ptr;}
             T&operator[](int x)const noexcept{return ptr[x];} 
             T*release(){T*t=ptr;ptr=nullptr;return t;}
        };
    }
}//namespace veritas
