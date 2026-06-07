#include"conf/conf.hpp"
#include"camera/pose.hpp"
#include<cstdio>
template<typename T>
HD Pose<T>::Pose():p(),f(0,0,-1),u(0,1,0),r(1,0,0){};
template<typename T>
HD Pose<T>::Pose(const vec3<T,P>&p1,const vec3<T,V>&f1,const vec3<T,V>&u1,const vec3<T,V>&r1):p(p1),f(f1),u(u1),r(r1){};
template<typename T>    
HD Pose<T>Pose<T>::init(){return Pose();}
template<typename T>
HD Pose<T>Pose<T>::look(const vec3<T,P>&pos,const vec3<T,P>&to,const vec3<T,V>&wd){
Pose ans;vec3<T,V>dir=nor(to-pos);ans.p=pos;
if(fabs(dot(dir,wd))>T(0.9999)){vec3<T,V>alt=dir.z<T(0.9)?vec3<T,V>(0,0,-1):vec3<T,V>(1,0,0);ans.r=nor(cs(dir,alt));}
else ans.r=nor(cs(dir,wd));ans.f=dir;ans.u=cs(ans.r,ans.f);return ans;}//look at
template<typename T>    
HD void Pose<T>::axis(const vec3<T,V>&v,T ang){quat<T>q=quat<T>::axis(v,ang);f=quat<T>::rot(q,f);u=quat<T>::rot(q,u);r=quat<T>::rot(q,r);}//rounte by axis
template<typename T>    
HD void Pose<T>::mv(const vec3<T,V>&v){mat34<T>m=mat34<T>::mv(v);f=m*f,u=m*u,r=m*r;}//translate
template<typename T>
HD void Pose<T>::mv(const vec3<T,P>&v){p=p+v;}
template<typename T>    
HD void Pose<T>::trs_fps(T dx,T dy,T dz){p=p+r*dx+u*dy+f*dz;}
template<typename T>    
HD void Pose<T>::rot_fps(T th,T phi,const vec3<T,V>&wd){
if(th!=T(0)){quat<T>qr=quat<T>::axis(r,th);u=quat<T>::rot(qr,u);r=quat<T>::rot(qr,r);f=quat<T>::rot(qr,f);}
if(phi!=T(0)){quat<T>qwd=quat<T>::axis(wd,phi);r=quat<T>::rot(qwd,r);f=quat<T>::rot(qwd,f);u=quat<T>::rot(qwd,u);}clean();}
template<typename T>    
HD void Pose<T>::orbit(const vec3<T,P>&ctr,const vec3<T,V>&a,const T ang){quat<T>q=quat<T>::axis(nor(a),ang);  
vec3<T,V>v=p-ctr;p=ctr+quat<T>::rot(q,v);r=quat<T>::rot(q,r);u=quat<T>::rot(q,u);f=quat<T>::rot(q,f);}
template<typename T>      
HD void Pose<T>::at(const vec3<T,P>&t,const vec3<T,V>&wd){f=nor(t-p);r=nor(cs(f,wd));u=cs(f,r);}
template<typename T>    
HD void Pose<T>::clean(){f=nor(f);r=nor(cs(u,f));u=cs(f,r);}
template<typename T>    
HD void Pose<T>::apply(const mat34<T>&m){r=nor(m*r);u=nor(m*u);f=nor(m*f);p=m*p;clean();}
template<typename T>    
HD quat<T>Pose<T>::to_quat(){mat34<T>m=mat34<T>::bas(r,u,-f,vec3<T,V>(0,0,0));quat<T>q=quat<T>::matr(m);return q;}
template<typename T>
HD Pose<T>Pose<T>::to_pos(const quat<T>&q,const vec3<T,P>&pos){
Pose<T>ans;ans.p=pos;ans.r=quat<T>::rot(q,vec3<T,V>(1,0,0));
ans.u=quat<T>::rot(q,vec3<T,V>(0,1,0));ans.f=quat<T>::rot(q,vec3<T,V>(0,0,-1));return ans;}
