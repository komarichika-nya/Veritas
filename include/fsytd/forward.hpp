#pragma once
namespace veritas{
    namespace fsytd{
        template<bool B>struct bool_type{static constexpr bool value=B;};
        using true_type=bool_type<true>;using false_type=bool_type<false>;
        template<typename T>struct remove_reference{using type=T;};
        template<typename T>struct remove_reference<T&>{using type=T;};
        template<typename T>struct remove_reference<T&&>{using type=T;};
        template<typename T>struct is_lvalue_reference:false_type{};
        template<typename T>struct is_lvalue_reference<T&>:true_type{};
        template<typename T>
        constexpr T&&forward_(typename remove_reference<T>::type&tp)noexcept{return static_cast<T&&>(tp);}
        template<typename T>
        constexpr T&&forward_(typename remove_reference<T>::type&&tp)noexcept{
          static_assert(!is_lvalue_reference<T>::value,"cannot forward an rvalue as a lvalue");
            return static_cast<T&&>(tp);
        }
    }//namespace fsytd
}//namespace veritas
