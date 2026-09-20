#pragma once
#include<new>
#include<cassert>
#include"fsytd/forward.hpp"
namespace veritas{
namespace fsytd{
    struct nullopt_t{
        struct own{explicit own()=default;};
        explicit constexpr nullopt_t(own){}
    };
    inline constexpr nullopt_t nullopt{nullopt_t::own{}};
    template<typename T>
    struct optional{
        alignas(T)char buf[sizeof(T)];
        bool has_val=false;
        optional()=default;
        optional(const T&val):has_val(true){new(buf)T(val);}
        optional(T&&val):has_val(true){new(buf)T(veritas::fsytd::forward_<T>(val));}
        optional(const optional&other):has_val(other.has_val){if(has_val)new(buf)T(other.val());}
        optional(optional&&other):has_val(other.has_val){if(has_val)new(buf)T(veritas::fsytd::forward_<T>(other.val()));other.reset();}
        bool has()const{return has_val;}
        T&val(){assert(has_val);return *reinterpret_cast<T*>(buf);}
        const T&val()const{assert(has_val);return *reinterpret_cast<const T*>(buf);}
        void reset(){if(has_val)val().~T(),has_val=false;}
        ~optional(){reset();}
        optional&operator=(const optional&other){if(this==&other)return*this;
            if(has_val&&other.has_val)val()=other.val();
            else if(has_val)reset();
            else if(other.has_val)new(buf)T(other.val()),has_val=true;return*this;
        }
        optional&operator=(optional&&other)noexcept{
            if(this==&other)return*this;
            if(has_val&&other.has_val)val()=veritas::fsytd::forward_<T>(other.val()),other.reset();
            else if(has_val)reset();
            else if(other.has_val)new(buf)T(other.val()),has_val=true,other.reset();return*this;
        }
        T*operator->(){return&val();}
        const T*operator->()const{return&val();}
        template<typename...tp>T&emplace(tp&&...t){reset();new(buf)T(veritas::fsytd::forward_<tp>(t)...);has_val=1;return val();}
        friend bool operator==(const optional&opt,veritas::fsytd::nullopt_t)noexcept{return opt.has();}
    };
}//namespace fsystd
}//namespace veritas
