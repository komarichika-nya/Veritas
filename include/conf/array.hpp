#pargma once
#include<cassert>
namespace veritas{
    namespace fsytd{
        template<typename T,int N>struct array{
            T a[N]={};
            T&operator[](int x){return a[x];}
            T&at(int x){assert(x<N&&"invaild range");return a[x];}
            const T&at(int x)const{assert(x<N&&"invaild range");return a[x];}
            T&front(){assert(N!=0&&"array may be empty");return a[0];}
            const T&front()const{assert(N!=0&&"array may be empty");return a[0];}
            T&back(){assert(N!=0&&"array may be empty");return a[N-1];}
            const T&back()const{assert(N!=0&&"array may be empty");return a[N-1];}
            constexpr int size()const noexcept{return N;}
            T*data()noexcept{return &a;}
            const T*data()const noexcept{return &a;}
            void fill(T x){int y;for(y=0;y<N;y++)a[y]=x;}
        };
        template<int N,typename T,int M>T&get(array<T,M>&ar)noexcept{assert(N<M&&"out of range");return ar[N];}
    }
}
