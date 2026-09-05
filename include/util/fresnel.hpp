#pragma once
namespace veritas{
    namespace fsytd{
        template<typename T>T fresnel(const vec3<T,V>&ii,const vec3<T,V>&nn,T er){
            auto i=nor(ii),n=nor(nn);Spectrum<T>L;
            T c=abs(dot(i,n));T eta=T(1)/er;T g2=(eta*eta)-T(1)+c*c;if(g2<T(0))return T(1);
            T g=sqrt(g2);T a=g-c,b=g+c;T f=T(0.5)*(a*a)/(b*b)*(T(1)+((c*b-T(1))*(c*b-T(1)))/((c*a+T(1))*(c*a+T(1))));
            assert(!std::isnan(f));return f;
        }
        template<typename T>T fresnel0(const vec3<T,V>&i,const vec3<T,V>&n,T eta,T k){
            T c=abs(dot(i,n));T eta2=eta*eta,k2=k*k;T c2=c*c;T rs=((eta2+k2)-T(2)*eta*c+c2)/((eta2+k2)+T(2)*eta*c+c2);
            T rp=((eta2+k2)*c2-T(2)*eta*c+T(1))/((eta2+k2)*c2+T(2)*eta*c+T(1));return (rs+rp)/T(2);
        }
    }//namespace fsytd
}//namespace veritas
