#pragma once
#include<new>
#include<cassert>
#include"conf/conf.hpp"
namespace fsystd{
    template<bool B>struct bool_type{static constexpr bool value=B;};
    using true_type=bool_type<true>;
    using false_type=bool_type<false>;
    template<typename T>struct remove_reference{using type=T;};
    template<typename T>struct remove_reference<T&>{using type=T;};
    template<typename T>struct remove_reference<T&&>{using type=T;};
    template<typename T>struct is_lvalue_reference:false_type{};
    template<typename T>struct is_lvalue_reference<T&>:true_type{};
    template<typename T>
    HD constexpr T&&forward_(typename remove_reference<T>::type&tp)noexcept{return static_cast<T&&>(tp);}
    template<typename T>
    HD constexpr T&&forward_(typename remove_reference<T>::type&&tp)noexcept{
    static_assert(!is_lvalue_reference<T>::value,"cannot forward an rvalue as a lvalue");
    return static_cast<T&&>(tp);}
    template<typename T>
    struct optional{
        alignas(T)char buf[sizeof(T)];
        bool has_val=false;
        HD optional()=default;
        HD optional(const T&val):has_val(true){new(buf)T(val);}
        HD optional(T&&val):has_val(true){new(buf)T(forward_<T>(val));}
        HD optional(const optional&other):has_val(other.has_val){if(has_val)new(buf)T(other.val());}
        HD optional(optional&&other):has_val(other.has_val){if(has_val)new(buf)T(forward_<T>(other.val()));other.reset();}
        HD bool has()const{return has_val;}
        T&val(){assert(has_val);return *reinterpret_cast<T*>(buf);}
        HD const T&val()const{assert(has_val);return *reinterpret_cast<const T*>(buf);}
        HD void reset(){if(has_val)val().~T(),has_val=false;}
        HD ~optional(){reset();}
        HD optional&operator=(const optional&other){if(this==&other)return*this;
            if(has_val&&other.has_val)val()=other.val();
            else if(has_val)reset();
            else if(other.has_val)new(buf)T(other.val()),has_val=true;return*this;
        }
        HD optional&operator=(optional&&other)noexcept{
            if(this==&other)return*this;
            if(has_val&&other.has_val)val()=forward_<T>(other.val()),other.reset();
            else if(has_val)reset();
            else if(other.has_val)new(buf)T(other.val()),has_val=true,other.reset();return*this;
        }
        HD T*operator->(){return&val();}
        HD const T*operator->()const{return&val();}
        template<typename...tp>HD T&emplace(tp&&...t){reset();new(buf)T(forward_<tp>(t)...);has_val=1;return val();}
    };
}//namespace fsystd
