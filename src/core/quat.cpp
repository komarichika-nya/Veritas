#include"conf/conf.hpp"
#include"core/quat.hpp"
#include<cmath>
template<typename T>HD quat<T>::quat():w(1),x(0),y(0),z(0){};
template<typename T>HD quat<T>::quat(T w,T x,T y,T z):w(w),x(x),y(y),z(z){}; 
template<typename T>HD quat<T>quat<T>::unit(){return{T(1),T(0),T(0),T(0)};}
template<typename T>HD quat<T>quat<T>::norm(const quat<T>&a){T v=sqrt(a.w*a.w+a.x*a.x+a.y*a.y+a.z*a.z);return{a.w/v,a.x/v,a.y/v,a.z/v};}
template<typename T>HD quat<T>quat<T>::axis(const vec3<T,V>&a,const T ang){auto na=nor(a);return norm({cos(ang/T(2)),sin(ang/T(2))*na.x,sin(ang/T(2))*na.y,sin(ang/T(2))*na.z});}
template<typename T>HD quat<T>quat<T>::vec(const vec3<T,V>&a,const vec3<T,V>&b){
auto na=nor(a);auto nb=nor(b);T v=dot(a,b);if(v>T(1)-eps)return{T(1),T(0),T(0),T(0)};
if(v<T(-1)+eps){vec3<T,V>c=abs(na.x)<T(0.9)?vec3<T,V>{1,0,0}:vec3<T,V>{0,1,0};
quat n=nor(cs(na,c));return norm({T(0),n.x,n.y,n.z});}vec3<T,V>c=cs(na,nb);return norm({T(1)+v,c.x,c.y,c.z});}
template<typename T>
template<typename tp>
HD vec3<T,tp>quat<T>::rot(const quat<T>&q,const vec3<T,tp>&v){vec3<T,V>qv(q.x,q.y,q.z);return v+T(2)*q.w*cs(qv,v)+T(2)*cs(qv,cs(qv,v));}
template<typename T>HD quat<T>quat<T>::matr(const mat34<T>&v){
T t=v.mat[0][0]+v.mat[1][1]+v.mat[2][2];quat<T>ans;
if(t>0){T s=T(2)*sqrt(t+T(1)),s1=T(1)/s;ans.w=T(0.25)*s;
ans.x=(v.mat[2][1]-v.mat[1][2])*s1;ans.y=(v.mat[0][2]-v.mat[2][0])*s1;ans.z=(v.mat[1][0]-v.mat[0][1])*s1;}
else if(v.mat[0][0]>v.mat[1][1]&&v.mat[0][0]>v.mat[2][2]){T s=T(2)*sqrt(T(1)+v.mat[0][0]-v.mat[1][1]-v.mat[2][2]),s1=T(1)/s;ans.w=(v.mat[2][1]-v.mat[1][2])*s1;ans.x=T(0.25)*s;
ans.y=(v.mat[0][1]+v.mat[1][0])*s1;ans.z=(v.mat[0][2]+v.mat[2][0])*s1;}
else if(v.mat[1][1]>v.mat[2][2]){T s=T(2)*sqrt(T(1)+v.mat[1][1]-v.mat[0][0]-v.mat[2][2]),s1=T(1)/s;
ans.w=(v.mat[0][2]-v.mat[2][0])*s1;ans.x=(v.mat[0][1]+v.mat[1][0])*s1;ans.y=T(0.25)*s;ans.z=(v.mat[1][2]+v.mat[2][1])*s1;}
else{T s=T(2)*sqrt(T(1)+v.mat[2][2]-v.mat[0][0]-v.mat[1][1]),s1=T(1)/s;ans.w=(v.mat[1][0]-v.mat[0][1])*s1;
ans.x=(v.mat[0][2]+v.mat[2][0])*s1;ans.y=(v.mat[1][2]+v.mat[2][1])*s1;ans.z=T(0.25)*s;}return norm(ans);}
template<typename T>HD quat<T>quat<T>::conj()const{return{w,-x,-y,-z};}    
template<typename T>HD quat<T>quat<T>::operator*(const quat<T>&a)const{
vec3<T,V>v1(x,y,z);vec3<T,V>v2(a.x,a.y,a.z);vec3<T,V>c=cs(v1,v2);
return{w*a.w-dot(v1,v2),w*v2.x+a.w*v1.x+c.x,w*v2.y+a.w*v1.y+c.y,w*v2.z+a.w*v1.z+c.z};}
template<typename T>HD quat<T>&quat<T>::operator*=(const quat&v){*this=(*this)*v;return *this;}
template<typename T>HD quat<T>slerp(quat<T>&a,quat<T>&b,T t){
a=quat<float>::norm(a);b=quat<float>::norm(b);T d=a.w*b.w+a.x*b.x+a.y*b.y+a.z*b.z;
// printf("%f\n",d);
d=max(T(-1),min(T(1),d));if(d<T(0)){d=-d;b={-b.w,-b.x,-b.y,-b.z};}
if(d>T(0.99995)){quat<T>r={a.w+(b.w-a.w)*t,a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t,a.z+(b.z-a.z)*t};return r;}
T th=acos(a.w*b.w+a.x*b.x+a.y*b.y+a.z*b.z);T v1=sin((1-t)*th)/sin(th),v2=sin(t*th)/sin(th);
return quat<T>(a.w*v1+b.w*v2,a.x*v1+b.x*v2,a.y*v1+b.y*v2,a.z*v1+b.z*v2);}
template<typename T>
HD quat<T>quat<T>::operator-(const quat<T>&a)const{return{w-a.w,x-a.x,y-a.y,z-a.z};}
template<typename T>
HD bool quat<T>::operator==(const quat<T>&a)const{return w-a.w<eps&&x-a.x<eps&&y-a.y<eps&&z-a.z<eps;}
template<typename T>
HD quat<T>quat<T>::operator-()const{return{-w,-x,-y,-z};}
template<typename T>
HD bool quat<T>::same(const quat<T>&a,const quat<T>&b){return a==b||a==-b;}
template<typename T>
HD T quat<T>::qlen(const quat<T>&a){return sqrt(a.w*a.w+a.x*a.x+a.y*a.y+a.z*a.z);}
