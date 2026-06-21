#include"camera/camera.hpp"
#include"camera/pose.hpp"
#include"sample/Independent.hpp"
#include"color/rgb.hpp"
#include<random>
#include"core/vec.hpp"
#include<omp.h>
#include<cstdio>
#include"util/mesh.hpp"
#include"util/surface.hpp"
#include<cassert>
#include<vector>
const int w=800,h=600;
Pose<float>pos=Pose<float>::init();
DOF<float>lens(20,0.1);
Box<float>f(vec2<float,P>(0.5,0.5));
bound2<int>b(vec2<int,P>(0,0),vec2<int,P>(w-1,h-1));
Film<Box<float>,float>film(vec2<int,P>(w,h),b);
shut<float>sh(0,0);
Projective<float,DOF<float>,Box<float>>cam(pos,lens,sh,film,w,h,45.0f);
Independent<float>in;
template<typename T>
T hit1(const ray<T>*r,T R,const vec3<T,P>&c1){
    vec3<T,V>oc=r->o-c1;
    T a=r->d.x*r->d.x+r->d.y*r->d.y+r->d.z*r->d.z;
    T b=T(2)*(r->d.x*oc.x+r->d.y*oc.y+r->d.z*oc.z);
    T c=oc.x*oc.x+oc.y*oc.y+oc.z*oc.z-R*R;
    T dt=b*b-T(4)*a*c;if(dt<T(0))return T(-1e8);
    T s=sqrt(dt);T s1=(-b+s)/(T(2)*a),s2=(-b-s)/(T(2)*a);
    T mx=max(s1,s2),mn=min(s1,s2);
    if(mn>eps)return mn;
    if(mx>eps)return mx;
    return T(-1e8);
}
template<typename T>
T hit3(const ray<T>*r,const vec3<T,P>&c,const vec3<T,V>&E1,const vec3<T,V>&E2){
vec3<T,V>nl=cs(E1,E2);T dn=dot(r->d,nl);if(abs(dn)<eps)return T(-1);T t=dot(c-r->o,nl)/dn;if(t<=eps)return T(-1);
vec3<T,V>rel=(r->o+r->d*t)-c;T a11=dot(E1,E1),a12=dot(E1,E2),a22=dot(E2,E2),b1=dot(rel,E1),b2=dot(rel,E2),det=a11*a22-a12*a12;
if(abs(det)<eps)return T(-1);T a=(b1*a22-b2*a12)/det,b=(a11*b2-a12*b1)/det;if(a<T(0)||a>T(1)||b<T(0)||b>T(1))return T(-1);return t;}
using std::isnan;
template<typename T>
T hit2(const ray<T>*r){if(abs(r->d.y)>eps)return -r->o.y/r->d.y;return T(-1e8);}
template<typename T>
vec3<T,P>sun(0,20,0);
template<typename T>
vec3<T,P>p1(4,15,2);
template<typename T>
vec3<T,P>p2(0,15,4);
template<typename T>
Spectrum<T>sc(10,10,10);
template<typename T>
vec3<T,V>e1(8,0,0);
template<typename T>
vec3<T,V>e2(0,0,-8);
template<typename T>
vec3<T,P>sun2(4,10,-10);
template<typename T>
vec3<T,V>e2_1(3,0,0);
template<typename T>
vec3<T,V>e2_2(0,5,0);
template<typename T>
Spectrum<T>sc2(5,5,5);
template<typename T>
vec3<T,V>sp(const vec3<T,V>&n,Independent<T>&s){
    T u=s.get1d(),v=s.get1d();T phi=2.0*3.1415926535*u;
    T r=sqrt(v);assert(!isnan(r));T x=cos(phi)*r,z=sin(phi)*r,y=sqrt(max(0.0,1.0-v));vec3<T,V>u1;
    if(abs(n.y)<T(0.9999))u1=cs(vec3<T,V>(0,1.0,0),n);
    else u1=nor(cs(vec3<T,V>(1.0,0,0),n));
    vec3<T,V>v1=cs(n,u1);return x*u1+y*n+z*v1;
}
template<typename T>
bool ref(const vec3<T,V>&wi,const vec3<T,V>&n,T eta,vec3<T,V>&wo){
T ci=-dot(wi,n),s2t=eta*eta*(T(1)-ci*ci);if(s2t>T(1))return 0;
T ct=sqrt(max(T(0),T(1)-s2t));wo=wi*eta+(eta*ci-ct)*n;return 1;}
template<typename T>
T Fr(T cosv,T f0){T x=T(1)-cosv;return f0+(T(1)-f0)*x*x*x*x*x;}
template<typename T>
vec3<T,V>mix(const vec3<T,V>&wi,const vec3<T,V>&n,T ro,int dep){vec3<T,V>w=wi+sp(n,in)*ro;return nor(w);}
template<typename T>
vec3<T,V>reflect(const vec3<T,V>&wi,const vec3<T,V>&n){return wi-T(2)*dot(wi,n)*n;} 
template<typename T>
Spectrum<T>render(const ray<T>*r,int dep){if(dep>20)return Spectrum<T>(0,0,0);
T h1=hit1(r,3.0f,vec3<T,P>(0,3,0)),h2=hit2(r),t=T(1e30);int tp=-1;if(h1>eps&&h1<t){t=h1;tp=0;}
if(h2>eps&&h2<t){t=h2;tp=1;}if(tp==-1){T u=T(0.5)*(r->d.y+T(1.0));return Spectrum<T>::lerp(Spectrum<T>(1,1,1),Spectrum<T>(0.3,0.5,0.7),u);}
vec3<T,P>p=r->o+r->d*t;Spectrum<T>rho;vec3<T,V>n;if(tp==0){vec3<T,V>wi=nor(r->d);n=nor(p-vec3<T,V>(0,3,0));
vec3<T,V>nn=n;T etai=T(1),etat=T(1.5),ci=-dot(wi,nn);if(ci<T(0)){nn=-nn;std::swap(etai,etat);ci=-ci;}
T eta=etai/etat;vec3<T,V>wo;T f0=std::pow((etai-etat)/(etai+etat),T(2));assert(ci>=0);T fr=Fr(ci,f0);T ro=T(0.03);
Independent<T>local=in.clone(dep);T xi=local.get1d();vec3<T,V>nxt;bool pth;
if(xi<fr){pth=1;nxt=reflect(wi,nn);nxt=mix(nxt,nn,ro,dep);}
else{pth=0;if(!ref(wi,nn,eta,wo))nxt=reflect(wi,nn);else nxt=mix(wo,nn,ro,dep);}
ray<T>ra;if(dot(nxt,nn)>0)ra.o=p+nn*eps;else ra.o=p-nn*eps;ra.d=nxt;Spectrum<T>li=render(&ra,dep+1);
T ob=len(ra.o-r->o);Spectrum<T>col(exp(-0.8*ob),exp(-0.05*ob),exp(-0.02*ob));
return col*li;}if(tp==1){n=vec3<T,V>(0,1,0);int ck=(int(floor(p.x))+int(floor(p.z)));rho=ck&1?Spectrum<T>(0.2,0.2,0.2):Spectrum<T>(0.8,0.8,0.8);}
//printf("o(%f,%f,%f) d:(%f %f %f) h1=%f h2=%f tp=%d\n",r->o.x,r->o.y,r->o.z,r->d.x,r->d.y,r->d.z,h1,h2,tp);
Independent<T>local=in.clone(dep);T G=T(1);
T u=local.get1d(),v=local.get1d();vec3<T,V>rd=u*e1<T>+v*e2<T>;vec3<T,P>pt=sun<T>+vec3<T,P>(rd.x,rd.y,rd.z);
vec3<T,V>dir=pt-p;vec3<T,V>n1=nor(cs(e1<T>,e2<T>));T s=len(cs(e1<T>,e2<T>));T dis=max(len(dir),eps);dir=nor(dir);Spectrum<T>le(0,0,0);
T s1=dot(dir,n);T s2=dot(dir,n1);if(s1>T(0)&&s2>T(0)){G=s1*s2/(dis*dis);le=rho/T(3.14159265)*G*sc<T>*s;}
ray<T>r1;vec3<T,V>nw=n*eps;r1.o=p+vec3<T,P>(nw.x,nw.y,nw.z);r1.d=sp(n,local);Spectrum<T>li=render(&r1,dep+1);
return rho*li+le;}
template<typename T>
T etai=T(1);
template<typename T>
T etat=T(1.5);
template<typename T>
T X(T a){return a>T(0)?T(1):T(0);}
template<typename T>
constexpr T aph=T(0.2);
template<typename T>
vec3<T,V>sph(const vec3<T,V>&n,Independent<T>&s){
T u=s.get1d(),v=s.get1d();T theta=atan(sqrt(-aph<T>*aph<T>*log(max(1-u,T(1e-9)))));
T phi=T(2)*T(3.1415926535)*v;T st=sin(theta),ct=cos(theta);vec3<T,V>t=abs(n.z)>T(0.9999)?cs(vec3<T,V>(1,0,0),n):cs(vec3<T,V>(0,0,1),n);
vec3<T,V>b=cs(n,t);return nor(st*cos(phi)*t+ct*n+st*sin(phi)*b);}
template<typename T>
T G(const vec3<T,V>&v,const vec3<T,V>&m,const vec3<T,V>&n,T a){
T vm=dot(v,m),vn=dot(v,n);if(vm<=T(0)||vn<=T(0))return T(0);T b=T(1)+sqrt(1+a*a*(1-vn*vn)/(vn*vn));return T(2)/b;}
template<typename T>
T D(const vec3<T,V>&n,const vec3<T,V>&m){
T cosm=dot(n,m);if(cosm<=T(0))return T(0);T tanm=(1-cosm*cosm)/(cosm*cosm);
T x=X(cosm);T d=x/(T(3.1415926535)*aph<T>*aph<T>*cosm*cosm*cosm*cosm)*exp((-tanm)/(aph<T>*aph<T>));assert(!isnan(d));return d;}
template<typename T>
T F(const vec3<T,V>&ii,const vec3<T,V>&nn,T er){auto i=nor(ii),n=nor(nn);Spectrum<T>L;
T c=abs(dot(i,n));T eta=T(1)/er;T g2=(eta*eta)-T(1)+c*c;if(g2<T(0))return T(1);T g=sqrt(g2);
T a=g-c,b=g+c;T f=T(0.5)*(a*a)/(b*b)*(T(1)+((c*b-T(1))*(c*b-T(1)))/((c*a+T(1))*(c*a+T(1))));assert(!isnan(f));return f;}
template<typename T>
vec3<T,V>refract(const vec3<T,V>&i,const vec3<T,V>&n,T eta){assert(!isnan(eta));T cosi=dot(i,n);
if(cosi<T(0)){T k=T(1)-eta*eta*(T(1)-cosi*cosi);if(k<T(0))return vec3<T,V>(0,0,0);return i*eta-(eta*cosi+sqrt(max(k,eps)))*n;}
else{eta=T(1)/eta;assert(!isnan(eta));vec3<T,V>nn=-n;cosi=-cosi;T k=T(1)-eta*eta*(T(1)-cosi*cosi);if(k<T(0))return vec3<T,V>(0,0,0);return i*eta-(eta*cosi+sqrt(max(k,eps)))*nn;}}
template<typename T>
vec3<T,V>vndf(const vec3<T,V>&i,T a,T u1,T u2){vec3<T,V>vh=nor(vec3<T,V>(i.x*a,i.y*a,i.z));T sq=vh.x*vh.x+vh.y*vh.y;
vec3<T,V>v1=sq>T(0)?(vec3<T,V>(-vh.y,vh.x,0)/T(sqrt(sq))):vec3<T,V>(1,0,0);vec3<T,V>v2=cs(vh,v1);T r=sqrt(u1);
T phi=T(2)*T(3.1415926535)*u2;T t1=r*cos(phi),t2=r*sin(phi);T s=T(0.5)*(T(1)+vh.z);t2=(T(1)-s)*sqrt(max(1-t1*t1,T(0)))+s*t2;
vec3<T,V>nh=t1*v1+t2*v2+sqrt(max(T(0),T(1)-t1*t1-t2*t2))*vh;return nor(vec3<T,V>(nh.x*a,nh.y*a,max(T(0),nh.z)));}
template<typename T>
vec2<T,P>ro(const vec3<T,V>&d){return{T(0.5)+T(atan2(d.z,d.x))/(T(2)*T(3.1415926535)),T(0.5)-T(asin(d.y))/T(3.1415926535)};}
template<typename T>
Spectrum<T>Fs(const vec3<T,V>&ii,const vec3<T,V>&nn,const Spectrum<T>&er){
assert(!isnan(er[0])&&!isnan(er[1])&&!isnan(er[2]));Spectrum<T>out(0,0,0);int x;for(x=0;x<3;x++)out[x]=F(ii,nn,er[x]);return out;}
int cnt=0,to=0;vec3<float,P>ctr(0,3,0);float R=3.0f;
double albedo(const vec3<float,V>&i,float a,int N){
    double acc;int x;
    for(x=0;x<N;x++){
        vec3<float,V>h=nor(vndf(i,a,in.get1d(),in.get1d()));
        float f=F(i,h,1.0f/1.5f);
        vec3<float,V>o;float w;float rd=in.get1d();
        if(rd<f){o=nor(-reflect(i,h));w=G(o,h,vec3<float,V>(0,0,1),a);}
        else{o=nor(refract(-i,h,1.0f/1.5f));w=G(o,-h,vec3<float,V>(0,0,-1),a);}
        if(dot(o,o)>0.0f)acc+=w;
    }
    return acc/N;
}
template<typename T>
Spectrum<T>render1(const ray<T>*r,int dep){
    //to++;
    if(dep>10)return Spectrum<T>(0,0,0);
    T h1=hit1(r,R,ctr),h2=hit2(r),t=T(1e30),h3=hit3(r,sun<T>,e1<T>,e2<T>);int tp=-1;
    if(h1>eps&&h1<t){t=h1;tp=0;}
    if(h2>eps&&h2<t){t=h2;tp=1;}if(h3>eps&&h3<t){t=h3;tp=2;}
    if(tp==-1){
        T u=T(0.5)*(r->d.y+T(1));
        return Spectrum<T>::lerp(Spectrum<T>(0.05,0.17,0.1),Spectrum<T>(0.03,0.04,0.06),u);
    }
    vec3<T,P>p=r->o+r->d*t;
    vec3<T,V>n,d;
    Spectrum<T>rho(0,0,0),li(0,0,0);
    Independent<T>local=in.clone(dep);
    if(tp==0){//object
        d=nor(p-ctr);//rho=Spectrum<T>(0.0001,0.0001,0.0001);
        n=d;bool etr=dot(r->d,n)<T(0);
        n=etr?n:-n;
        vec2<T,P>uv=ro(d);
        int cx=int(uv.x*T(20)),cy=int(uv.y*T(20));
        T dc=aph<T>;//(cx+cy)&1?T(0.1):T(0.001);
        vec3<T,V>i=nor(-r->d);T ei=etr?etai<T>:etat<T>;T et=etr?etat<T>:etai<T>;T er=ei/et;
        vec3<T,V>tt=abs(n.z)>T(0.9999)?nor(cs(vec3<T,V>(1,0,0),n)):nor(cs(vec3<T,V>(0,0,1),n));
        vec3<T,V>bb=cs(n,tt);vec3<T,V>il(dot(i,tt),dot(i,bb),dot(i,n));vec3<T,V>hl=nor(vndf(nor(il),dc,local.get1d(),local.get1d()));
        vec3<T,V>h=hl.x*tt+hl.y*bb+hl.z*n;h=nor(h);T rd=local.get1d();T f=F(i,h,er);if(rd<f){vec3<T,V>o=nor(-reflect(i,h));
        T fr=G(o,h,n,dc);ray<T>ra;ra.o=p+vec3<T,P>(n.x,n.y,n.z)*eps;
        ra.d=o;return render1(&ra,dep+1)*fr;}else{auto v=refract(-i,h,er);
        if(v.x==T(0)&&v.y==T(0)&&v.z==T(0)){vec3<T,V>o=nor(-reflect(i,h));T fr=G(o,h,n,dc);
        ray<T>ra;ra.o=p+n*eps;ra.d=o;return render1(&ra,dep+1)*fr;}
        vec3<T,V>o=nor(v);assert(!isnan(o.x)&&!isnan(o.y)&&!isnan(o.z));T ft=G(o,-h,-n,dc);//printf("%f %f %f\n",ft,ft_a,ft_b);
        assert(!isnan(ft));ray<T>ra;ra.o=p-vec3<T,V>(n.x,n.y,n.z)*eps;ra.d=o;return render1(&ra,dep+1)*ft;}
    }
        if(tp==1){//ground
            //return Spectrum<T>(1,1,1);
            n=vec3<T,V>(0,1,0);int ck=(int(floor(p.x))+int(floor(p.z)));rho=ck&1?Spectrum<T>(0.2,0.2,0.2):Spectrum<T>(0.8,0.8,0.8);
            T rr=local.get1d();
            vec3<T,V>i=nor(-r->d);vec3<T,V>tt=nor(cs(vec3<T,V>(0,0,1),n));
            vec3<T,V>bb=cs(n,tt);vec3<T,V>il(dot(i,tt),dot(i,bb),dot(i,n));vec3<T,V>hl=nor(vndf(nor(il),T(0.5),local.get1d(),local.get1d()));
            vec3<T,V>h=hl.x*tt+hl.y*bb+hl.z*n;h=nor(h);T f=T(0.3);if(rr<f){vec3<T,V>o=nor(-reflect(i,h));
            ray<T>rs;rs.o=p+n*eps;rs.d=o;li=render1(&rs,dep+1)*rho;}else{ray<T>r1;r1.o=p+n*eps;r1.d=sp(n,local);li=render1(&r1,dep+1)*rho;}
        }
        if(tp==2){return sc<T>;}
        auto add=[&](const vec3<T,P>&sunn,const vec3<T,V>&en1,const vec3<T,V>&en2,const Spectrum<T>&scn)->Spectrum<T>{
        int spp=100;Spectrum<T>tot(0,0,0);for(int x=0;x<spp;x++){
        T Gi=T(0);T u=local.get1d(),v=local.get1d();
        vec3<T,V>rd=u*en1+v*en2;vec3<T,P>pt=sunn+vec3<T,P>(rd.x,rd.y,rd.z);
        vec3<T,V>dir=pt-p;vec3<T,V>n1=nor(cs(en1,en2));T s=len(cs(en1,en2));T dis=max(len(dir),eps);dir=nor(dir);
        //ray<T>r1;r1.o=p+n*eps;r1.d=dir;auto ck1=hit1(&r1,T(3),vec3<T,P>(-1,4,1)),ck2=hit2(&r1);if((ck1>eps&&ck1<dis-eps)||(ck2>eps&&ck2<dis-eps))continue;
        Spectrum<T>brdf;
        if(tp==1){
            brdf=rho/T(3.1415926535);assert(!isnan(brdf[0])&&!isnan(brdf[1])&&!isnan(brdf[2]));}
        else{
            vec3<T,V>i=nor(-r->d);vec3<T,V>hh=nor(i+dir);
            T ch=dot(n,hh);if(ch<=eps)continue;
            T ch2=ch*ch;T a2=aph<T>*aph<T>;assert(!isnan(a2));
            T v=a2+(T(1)-ch2)/ch2,dem=T(3.1415926535)*ch2*ch2*v*v;
            if(dem<=eps||!std::isfinite(dem))continue;T d=a2/dem;
            //printf("%f %f %f %f\n",ch2,ch,dem,pow((a2+(T(1)-ch2)/ch2),T(2)));
            assert(!isnan(d));
            T Gg=G(i,hh,n,aph<T>)*G(dir,hh,n,aph<T>);T den=max(eps,T(4)*abs(dot(i,hh))*abs(dot(dir,hh)));
            Spectrum<T>ei(1,1,1);Spectrum<T>et(1.5,1.5,1.5);
            Spectrum<T>f=Fs(i,hh,ei/et);//assert(!isnan(f));
            brdf=f*Gg*d/den;}T s1=dot(dir,n),s2=dot(dir,n1);
            if(s1>T(0)&&s2>T(0)){Gi=s1*s2/(dis*dis);assert(!isnan(Gi));assert(!isnan(brdf[0])&&!isnan(brdf[1])&&!isnan(brdf[2]));tot=tot+brdf*Gi*scn*s;}
        }
            return tot/T(spp);
        };
        Spectrum<T>le=add(sun<T>,e1<T>,e2<T>,sc<T>);
        return li;//+le;
}
template<typename T>
Spectrum<T>F0(const vec3<T,V>&i,const vec3<T,V>&n,const Spectrum<T>&eta,const Spectrum<T>&k){
T c=abs(dot(i,n));Spectrum<T>eta2=eta*eta,k2=k*k;T c2=c*c;Spectrum<T>rs=((eta2+k2)-T(2)*eta*c+c2)/((eta2+k2)+T(2)*eta*c+c2);
Spectrum<T>rp=((eta2+k2)*c2-T(2)*eta*c+T(1))/((eta2+k2)*c2+T(2)*eta*c+T(1));return (rs+rp)/T(2);}
template<typename T>
Spectrum<T>render2(const ray<T>*r,int dep){
    if(dep>10)return Spectrum<T>(0,0,0);
    T h1=hit1(r,3.0f,vec3<T,P>(0,3,0)),h2=hit2(r),t=T(1e30);int tp=-1;
    if(h1>eps&&h1<t){t=h1;tp=0;}if(h2>eps&&h2<t){t=h2;tp=1;}Spectrum<T>li;
    if(tp==-1){T u=T(0.5)*(r->d.y+T(1.0));return Spectrum<T>(1,1,1);}//return Spectrum<T>::lerp(Spectrum<T>(0.05,0.07,0.1),Spectrum<T>(0.03,0.04,0.06),u);}
    vec3<T,P>p=r->o+r->d*t;vec3<T,V>n;Spectrum<T>rho(0,0,0);Independent<T>local=in.clone(dep);
    if(tp==0){//object
        n=nor(p-vec3<T,P>(0,3,0));rho=Spectrum<T>(0.5,0.5,0.5);
        vec3<T,V>i=nor(-r->d);
        vec3<T,V>tt=abs(n.z)>T(0.9999)?nor(cs(vec3<T,V>(1,0,0),n)):nor(cs(vec3<T,V>(0,0,1),n));
        vec3<T,V>bb=cs(n,tt);vec3<T,V>il(dot(i,tt),dot(i,bb),dot(i,n));vec3<T,V>hl=nor(vndf(nor(il),T(0.1),local.get1d(),local.get1d()));vec3<T,V>h=hl.x*tt+hl.y*bb+hl.z*n;vec3<T,V>o=nor(-reflect(i,h));
        T rd=local.get1d();Spectrum<T>f=F0(i,h,Spectrum<T>(0.271,0.956,1.327),Spectrum<T>(3.609,2.458,2.004));Spectrum<T>fr_a=(f*G(i,h,n,aph<T>)*G(o,h,n,aph<T>)*abs(dot(o,h)));T fr_b=max(abs(dot(i,n))*abs(dot(h,n)),eps);
        Spectrum<T>fr=fr_a/fr_b;ray<T>ra;
        ra.o=p+vec3<T,P>(n.x,n.y,n.z)*eps;ra.d=o;
        li=render2(&ra,dep+1)*fr;
    }
    if(tp==1){//ground
        n=vec3<T,V>(0,1,0);rho=Spectrum<T>(0.15,0.15,0.17);//int ck=(int(floor(p.x))+int(floor(p.z)));rho=ck&1?Spectrum<T>(0.2,0.2,0.2):Spectrum<T>(0.8,0.8,0.8);
        T rr=local.get1d();
        vec3<T,V>i=nor(-r->d);vec3<T,V>tt=nor(cs(vec3<T,V>(0,0,1),n));
        vec3<T,V>bb=cs(n,tt);vec3<T,V>il(dot(i,tt),dot(i,bb),dot(i,n));vec3<T,V>hl=nor(vndf(nor(il),T(0.5),local.get1d(),local.get1d()));
        vec3<T,V>h=hl.x*tt+hl.y*bb+hl.z*n;h=nor(h);T f=T(0.3);if(rr<f){vec3<T,V>o=nor(-reflect(i,h));
        ray<T>rs;rs.o=p+n*eps;rs.d=o;li=render2(&rs,dep+1)*rho;}else{ray<T>r1;r1.o=p+n*eps;r1.d=sp(n,local);li=render2(&r1,dep+1)*rho;}
    }
    if(tp==2){return sc<T>;}
    auto add=[&](const vec3<T,P>&sunn,const vec3<T,V>&en1,const vec3<T,V>&en2,const Spectrum<T>&scn)->Spectrum<T>{
    int spp=16;Spectrum<T>tot(0,0,0);
    for(int x=0;x<spp;x++){
        T Gi=T(0);T u=local.get1d(),v=local.get1d();
        vec3<T,V>rd=u*en1+v*en2;vec3<T,P>pt=sunn+vec3<T,P>(rd.x,rd.y,rd.z);
        vec3<T,V>dir=pt-p;vec3<T,V>n1=nor(cs(en1,en2));T s=len(cs(en1,en2));T dis=max(len(dir),eps);dir=nor(dir);
        //ray<T>r1;r1.o=p+n*eps;r1.d=dir;auto ck1=hit1(&r1,T(3),vec3<T,P>(-1,3,1)),ck2=hit2(&r1);if((ck1>eps&&ck1<dis-eps)||(ck2>eps&&ck2<dis-eps))continue;
        Spectrum<T>brdf;if(tp==1){brdf=rho/T(3.1415926535);}else{vec3<T,V>i=nor(-r->d);vec3<T,V>hh=nor(i+dir);
        T ch=dot(n,hh);if(ch<=T(0))continue;T ch2=ch*ch;T a2=aph<T>*aph<T>;T d=a2/(T(3.1415926535)*ch2*ch2*pow((a2+(T(1)-ch2)/ch2),T(2)));assert(!isnan(d));
        T Gg=G(i,hh,n,aph<T>)*G(dir,hh,n,aph<T>);T den=max(eps,T(4)*abs(dot(i,n))*abs(dot(dir,n)));Spectrum<T>f=F0(i,hh,Spectrum<T>(0.271,0.956,1.327),Spectrum<T>(3.609,2.458,2.004));
        brdf=f*Gg*d/den;}T s1=dot(dir,n),s2=dot(dir,n1);
        if(s1>T(0)&&s2>T(0)){Gi=s1*s2/(dis*dis);tot=tot+brdf*Gi*scn*s;}}
        return tot/T(spp);
    };
    ray<T>r1;r1.o=p+n*eps;r1.d=sp(n,local);assert(dot(r1.d,n)>=0);
    Spectrum<T>le=add(sun<T>,e1<T>,e2<T>,sc<T>);
    return li;//+le;
}
int main(){
//printf("f:(%f %f %f) r:(%f %f %f) u:(%f %f %f)\n",cam.pos.f.x,cam.pos.f.y,cam.pos.f.z,cam.pos.r.x,cam.pos.r.y,cam.pos.r.z,cam.pos.u.x,cam.pos.u.y,cam.pos.u.z);
Projective<float,DOF<float>,Box<float>>cam(pos,lens,sh,film,w,h,45.0f);
cam.pos.mv(vec3<float,P>(0,5,20));
cam.pos=cam.pos.look(cam.pos.p,vec3<float,P>(0,6,0),vec3<float,V>(0,1,0));
//printf("f:(%f %f %f) r:(%f %f %f) u:(%f %f %f)\n",cam.pos.f.x,cam.pos.f.y,cam.pos.f.z,cam.pos.r.x,cam.pos.r.y,cam.pos.r.z,cam.pos.u.x,cam.pos.u.y,cam.pos.u.z);

int spp=200;
#pragma omp parallel for
for(int y=0;y<h;y++){
Independent<float>local=in.clone(y);
bound2<int>bd(vec2<int,P>(0,y),vec2<int,P>(w-1,y));
auto tile=cam.film->getTile(bd);
    for(int x=0;x<w;x++){
        local.sp(x,y,y*w+h,100);
        int s;Spectrum<float>sum(0,0,0),sumsp(0,0,0);
        for(s=0;s<spp;s++){
        cameraSample<float>cs; 
        cs.px=x+local.get1d();
        cs.py=y+local.get1d();
        local.get2d(&cs.lx,&cs.ly);
        cs.t=local.get1d();
        ray<float>r;
        cam.generateRay(&r,cs);
        Spectrum<float>L=render1(&r,2);
        sum=sum+L;
        }sum=sum/float(spp);
        tile.addSample(f,vec2<float,P>(x,y),sum,1.0f);
        //tile.addSplat(vec2<T,P>(x,y),sumsp,1.0f);
    }
    film.mergeTile(tile);
}
//double acc=albedo(vec3<float,V>(0,3,0),0.0238f,1000);
printf("%f\n",acc);
film.writeImage("b.ppm");
//printf("%f\n",(float)cnt/to);
return 0;
}
