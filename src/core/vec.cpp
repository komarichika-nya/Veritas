#include"conf/conf.hpp"
#include<cmath>
template<typename T,typename tag,int N>
HD vec<T,tag,N>::vec()=default;
template<typename T,typename tag,int N>
HD vec<T,tag,N>::vec(T x,T y)requires(N==2):vec_type<T,2>{x,y}{};
template<typename T,typename tag,int N>
HD vec<T,tag,N>::vec(T x,T y,T z)requires(N==3):vec_type<T,3>{x,y,z}{};
template<typename T,typename tag,int N>
template<typename tg>
HD vec<T,tag,N>::vec(const vec<T,tg,N>&v){int x;for(x=0;x<N;x++)(*this)[x]=v[x];}
template<typename T,typename tag,int N>    
HD T&vec<T,tag,N>::operator[](int x){return(&this->x)[x];}
template<typename T,typename tag,int N>    
HD const T&vec<T,tag,N>::operator[](int x)const{return(&this->x)[x];}
template<typename T,typename tag,int N>
HD vec<T,tag,N>vec<T,tag,N>::operator-()const{int x;vec<T,tag,N>ans;for(x=0;x<N;x++)ans[x]=-(*this)[x];return ans;}
template<typename T,typename tag,int N>
HD vec<T,tag,N>&vec<T,tag,N>::operator=(const vec<T,tag,N>&v){int x;for(x=0;x<N;x++)(*this)[x]=v[x];return *this;}
template<typename T,int N>
HD vec<T,V,N>operator-(const vec<T,P,N>&v1,const vec<T,P,N>&v2){vec<T,V,N>ans;int x;for(x=0;x<N;x++)ans[x]=v1[x]-v2[x];return ans;}
template<typename T,typename tag,int N,typename U>
HD vec<T,tag,N>operator*(U s,const vec<T,tag,N>&v){vec<T,tag,N>ans;int x;for(x=0;x<N;x++)ans[x]=v[x]*s;return ans;}
template<typename T,typename tag,int N>
HD T dot(const vec<T,tag,N>&v1,const vec<T,tag,N>&v2){T ans=0;int x;for(x=0;x<N;x++)ans+=v1[x]*v2[x];return ans;}
template<typename T,typename tag>
HD T cs(const vec<T,tag,2>&a,const vec<T,tag,2>&b){return a.x*b.y-a.y*b.x;}
template<typename T,typename tag>
HD vec<T,tag,3>cs(const vec<T,tag,3>&a,const vec<T,tag,3>&b){return{a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
template<typename T,typename tag,int N>
HD T len(const vec<T,tag,N>&v){T ans=0;int x;for(x=0;x<N;x++)ans+=v[x]*v[x];return sqrt(ans);}
template<typename T,typename tag,int N>
HD vec<T,tag,N>nor(const vec<T,tag,N>&v){return v/len(v);}
template<typename T>
HD T lerp(const T&a,const T&b,const T&t){return a*t+b*(1-t);}
template<typename T,typename tag,int N>
HD vec<T,tag,N>max(const vec<T,tag,N>&a,const vec<T,tag,N>&b){vec<T,tag,N>ans;int x;for(x=0;x<N;x++)ans[x]=a[x]>b[x]?a[x]:b[x];return ans;}
template<typename T,typename tag,int N>
HD vec<T,tag,N>min(const vec<T,tag,N>&a,const vec<T,tag,N>&b){vec<T,tag,N>ans;int x;for(x=0;x<N;x++)ans[x]=a[x]>b[x]?b[x]:a[x];return ans;}
template<typename T,typename tag,int N>
HD vec<T,tag,N>abs(const vec<T,tag,N>&a){vec<T,tag,N>ans;int x;for(x=0;x<N;x++)ans[x]=a[x]>0?a[x]:-a[x];return ans;}
template<typename T,typename tag,int N>
HD bool vec<T,tag,N>::operator==(const vec<T,tag,N>&a)const{bool ck=1;int x;for(x=0;x<N;x++)if((&this->x)[x]!=a[x])ck=0;return ck;}
template<typename T,typename tag>
using vec2=vec<T,tag,2>;
template<typename T,typename tag>
using vec3=vec<T,tag,3>;
