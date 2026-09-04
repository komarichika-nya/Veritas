#pragma once
#include"conf/conf.hpp"
#include"conf/lim.hpp"
#include"core/vec.hpp"
namespace veritas{
    template<typename T,int N>struct bound_type;
    template<typename T>struct bound_type<T,2>{vec2<T,P>mn,mx;};
    template<typename T>struct bound_type<T,3>{vec3<T,P>mn,mx;};
    template<typename T,int N>
    struct bound:bound_type<T,N>{
        using bound_type<T,N>::mx;
        using bound_type<T,N>::mn;
        bound()=default;
        bound(vec2<T,P>mn,vec2<T,P>mx)requires(N==2):bound_type<T,2>{mn,mx}{}   
        bound(vec3<T,P>mn,vec3<T,P>mx)requires(N==3):bound_type<T,3>{mn,mx}{}
        vec<T,P,N>&operator[](int x){return x==0?mn:mx;}
        const vec<T,P,N>&operator[](int x)const{return x==0?mn:mx;}   
        bound<T,N>make(const vec<T,P,N>&a,const vec<T,P,N>&b){return bound<T,N>(min(a,b),max(a,b));}    
        bound<T,N>add(const bound<T,N>&b,const vec<T,P,N>&p){return bound<T,N>(min(b.mn,p),max(b.mx,p));}
        bound<T,N>intersect(const bound<T,N>&b1,const bound<T,N>&b2){return bound<T,N>(max(b1.mn,b2.mn),min(b1.mx,b2.mx));}    
        bool inside(const bound<T,N>&b,const vec<T,P,N>&v){int x;for(x=0;x<N;x++){if(v[x]>b.mx[x]||v[x]<b.mn[x])return 0;}return 1;}
        T area()const{vec<T,P,N>dif;int x;T ans=T(1);for(x=0;x<N;x++)dif[x]=mx[x]-mn[x]+1,ans*=dif[x];return ans;}
        vec<T,P,N>center()const{vec<T,P,N>ans;int x;for(x=0;x<N;x++)ans[x]=(mx[x]+mn[x])/T(2);return ans;}
        bound<T,N>&operator=(const bound<T,N>&b){this->mx=b.mx;this->mn=b.mn;return *this;}
        bool intersectP(const vec3<T,P>&o,const vec3<T,V>&d,T*near,T*far){
            if(d.x<veritas::fsytd::lim<T>::eps()||d.y<veritas::fsytd::lim<T>::eps()||d.z<veritas::fsytd::lim<T>::eps())return 0;
            T dx=T(1)/d.x,dy=T(1)/d.y,dz=T(1)/d.z;
            T tx1=(mx.x-o.x)/dx,ty1=(mx.y-o.y)/dy,tz1=(mx.z-o.z)/dz;
            T tx2=(mn.x-o.x)/dx,ty2=(mn.y-o.y)/dy,tz2=(mn.z-o.z)/dz;
            T enter=veritas::fsytd::max(veritas::fsytd::lim<T>::min(),veritas::fsytd::max(tx1,veritas::fsytd::max(ty1,tz1)));
            T leave=veritas::fsytd::min(veritas::fsytd::lim<T>::max(),veritas::fsytd::min(tx2,veritas::fsytd::min(ty2,tz2)));
            if(enter>leave)veritas::fsytd::swap(enter,leave);near=enter;far=leave;
            return leave>=0&&enter<=leave;
        }
    };
    template<typename T>
    using bound2=bound<T,2>;
    template<typename T>
    using bound3=bound<T,3>;
}//namespace veritas
