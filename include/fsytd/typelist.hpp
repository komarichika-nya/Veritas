#pragma once
namespace veritas{
    namespace fsytd{
        template<typename...Ts>struct TypeList{};
        template<typename U,typename...Ts>constexpr int idx_of=-1;
        template<typename U,typename H,typename...Ts>constexpr int idx_of<U,H,Ts...> =std::is_same_v<U,H>?0:1+idx_of<U,Ts...>;
        //dispatch
        template<int I,typename F,typename H,typename...Ts>
        decltype(auto)dispatch(int tag,const void*p,F&&f){
            if constexpr(sizeof...(Ts)==0)return f(*static_cast<const H*>(p));
            else{if(tag==I)return f(*static_cast<const H*>(p));return dispatch<I+1,F,Ts...>(tag,p,veritas::fsytd::forward_<F>(f));}
        }
        template<int I,typename F,typename H,typename...Ts>
        decltype(auto)dispatch(int tag,void*p,F&&f){
            if constexpr(sizeof...(Ts)==0)return f(*static_cast<H*>(p));
            else{if(tag==I)return f(*static_cast<H*>(p));return dispatch<I+1,F,Ts...>(tag,p,veritas::fsytd::forward_<F>(f));}
        }
    }
}
