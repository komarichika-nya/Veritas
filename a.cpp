#include"camera/camera.hpp"
#include"camera/pose.hpp"
#include"sample/Independent.hpp"
#include"color/rgb.hpp"
#include<random>
#include"core/vec.hpp"
#include<omp.h>
#include<cstdio>
#include<cassert>
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
using std::isnan;
template<typename T>
T hit2(const ray<T>*r){if(abs(r->d.y)>eps)return -r->o.y/r->d.y;return T(-1e8);}
template<typename T>
vec3<T,P>sun(0,5,10);
template<typename T>
T sz=T(2);
template<typename T>
vec3<T,V>dir_m=nor(vec3<T,V>(0,3,0)-sun<T>);
template<typename T>
vec3<T,P>p1(6,7,2);
template<typename T>
vec3<T,P>p2(3,6,4);
template<typename T>
Spectrum<T>sc(1,1,1);
template<typename T>
vec3<T,V>e1_dir=nor(cs(dir_m<T>,vec3<T,V>(0,1,0)));
template<typename T>
vec3<T,V>e2_dir=nor(cs(dir_m<T>,e1_dir<T>));
template<typename T>
vec3<T,V>e1=e1_dir<T>*sz<T>;
template<typename T>
vec3<T,V>e2=e2_dir<T>*sz<T>;
template<typename T>
vec3<T,V>sp(const vec3<T,V>&n,Independent<T>&s){
    T u=s.get1d(),v=s.get1d();T phi=2.0*3.1415926535*u;
    T r=sqrt(v);assert(!isnan(r));T x=cos(phi)*r,z=sin(phi)*r,y=sqrt(max(0.0,1.0-v));vec3<T,V>u1;
    if(abs(n.x)>T(0.1))u1=nor(cs(vec3<T,V>(0,1.0,0),n));
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
vec3<T,V>mix(const vec3<T,V>&wi,const vec3<T,V>&n,T ro,int dep){vec3<T,V>w=wi+sp(n,dep)*ro;return nor(w);}
template<typename T>
vec3<T,V>reflect(const vec3<T,V>&wi,const vec3<T,V>&n){return wi-T(2)*dot(wi,n)*n;} 
template<typename T>
Spectrum<T>render(const ray<T>*r,int dep){
    if(dep>20)return Spectrum<T>(0,0,0);
    T h1=hit1(r,3.0f,vec3<T,P>(0,3,0)),h2=hit2(r),t=T(1e30);int tp=-1;
    if(h1>eps&&h1<t){t=h1;tp=0;}
    if(h2>eps&&h2<t){t=h2;tp=1;}
    if(tp==-1){T u=T(0.5)*(r->d.y+T(1.0));return Spectrum<T>::lerp(Spectrum<T>(1,1,1),Spectrum<T>(0.3,0.5,0.7),u);}
    vec3<T,P>p=r->o+r->d*t;Spectrum<T>rho;vec3<T,V>n;
    if(tp==0){vec3<T,V>wi=nor(r->d);n=nor(p-vec3<T,V>(0,3,0));
        vec3<T,V>nn=n;T etai=T(1),etat=T(1.5),ci=-dot(wi,nn);if(ci<T(0)){nn=-nn;std::swap(etai,etat);ci=-ci;}
        T eta=etai/etat;vec3<T,V>wo;T f0=std::pow((etai-etat)/(etai+etat),T(2));assert(ci>=0);T fr=Fr(ci,f0);T ro=T(0.03);
        Independent<T>local=in.clone(dep);T xi=local.get1d();vec3<T,V>nxt;bool pth;
        if(xi<fr){pth=1;nxt=reflect(wi,nn);nxt=mix(nxt,nn,ro,dep);}
        else{pth=0;if(!ref(wi,nn,eta,wo))nxt=reflect(wi,nn);else nxt=mix(wo,nn,ro,dep);}
        ray<T>ra;if(dot(nxt,nn)>0)ra.o=p+nn*eps;else ra.o=p-nn*eps;ra.d=nxt;Spectrum<T>li=render(&ra,dep+1);
        T ob=len(ra.o-r->o);Spectrum<T>col(exp(-0.8*ob),exp(-0.05*ob),exp(-0.02*ob));
        return col*li;
    }
    if(tp==1){n=vec3<T,V>(0,1,0);int ck=(int(floor(p.x))+int(floor(p.z)));rho=ck&1?Spectrum<T>(0.2,0.2,0.2):Spectrum<T>(0.8,0.8,0.8);}
    //printf("o(%f,%f,%f) d:(%f %f %f) h1=%f h2=%f tp=%d\n",r->o.x,r->o.y,r->o.z,r->d.x,r->d.y,r->d.z,h1,h2,tp);
    Independent<T>local=in.clone(dep);T G=T(1);
    T u=local.get1d(),v=local.get1d();vec3<T,V>rd=u*e1<T>+v*e2<T>;vec3<T,P>pt=sun<T>+vec3<T,P>(rd.x,rd.y,rd.z);
    vec3<T,V>dir=pt-p;vec3<T,V>n1=nor(cs(e1<T>,e2<T>));T s=len(cs(e1<T>,e2<T>));T dis=max(len(dir),eps);dir=nor(dir);Spectrum<T>le(0,0,0);
    T s1=dot(dir,n);T s2=dot(dir,n1);if(s1>T(0)&&s2>T(0)){G=s1*s2/(dis*dis);le=rho/T(3.14159265)*G*sc<T>*s;}
    ray<T>r1;vec3<T,V>nw=n*eps;r1.o=p+vec3<T,P>(nw.x,nw.y,nw.z);r1.d=sp(n,dep);Spectrum<T>li=render(&r1,dep+1);
    return rho*li+le;
}
template<typename T>
T etai=T(1);
template<typename T>
T etat=T(1.53);
template<typename T>
T X(T a){return a>T(0)?T(1):T(0);}
template<typename T>
constexpr T aph=T(0.0394);
template<typename T>
vec3<T,V>sph(const vec3<T,V>&n,Independent<T>&s){
T u=s.get1d(),v=s.get1d();T theta=atan(sqrt(-aph<T>*aph<T>*log(max(1-u,T(1e-9)))));
T phi=T(2)*T(3.1415926535)*v;T st=sin(theta),ct=cos(theta);vec3<T,V>t=abs(n.z)>T(0.9999)?cs(vec3<T,V>(0,0,1),n):cs(vec3<T,V>(1,0,0),n);
vec3<T,V>b=cs(n,t);
return nor(st*cos(phi)*t+ct*n+st*sin(phi)*b);}
template<typename T>
T G(const vec3<T,V>&v,const vec3<T,V>&m,const vec3<T,V>&n){
T vn=dot(v,n),vm=dot(v,m);if(vn<=T(0)||vm<=T(0))return T(0);
T tanv=sqrt(max(1-vn*vn,T(0)))/vn;if(tanv<T(1e-6))return T(1);
T a=T(1)/max((aph<T>*tanv)            assert(!isnan(fr_a));assert(!isnan(fr_b));

            T fr=fr_a/fr_b;

            ray<T>ra;

            ra.o=p+vec3<T,P>(n.x,n.y,n.z)*eps;

            ra.d=o;

            Spectrum<T>li=render1(&ra,dep+1);

            return li*fr;

        }

    else{

        n=nor(p-vec3<T,P>(0,3,0));

        vec3<T,V>i=nor(-r->d);

        vec3<T,V>h=nor(sph(n,local));

        auto v=refract(r->d,h);

        if(v.x==T(0)&&v.y==T(0)&&v.z==T(0)){

            o=nor(reflect(r->d,h));

            T fr_a=(G(i,h,n)*G(o,h,n)*abs(dot(o,h)));

            T fr_b=max((T(4)*abs(dot(i,n))*abs(dot(h,n))),eps);

            assert(!isnan(fr_a));assert(!isnan(fr_b));

            T fr=fr_a/fr_b;

            ray<T>ra;

            ra.o=p+n*eps;

            ra.d=o;

            Spectrum<T>li=render1(&ra,dep+1);

            return li*fr;

        }

        vec3<T,V>o=nor(v);

        assert(!isnan(o.x)&&!isnan(o.y)&&!isnan(o.z));

        T ft_a=abs(dot(i,h))*etat<T>*etat<T>*G(i,h,n)*G(o,h,n);

        T ft_b=max(T(4)*abs(dot(i,h))*abs(dot(o,n))*abs(dot(h,n))*pow((etai<T>*dot(i,h)+etat<T>*dot(o,h)),T(2)),eps);

    T ft=ft_a/ft_b;//printf("%f %f %f\n",ft,ft_a,ft_b);

    assert(!isnan(ft));ray<T>ra;ra.o=p+vec3<T,V>(n.x,n.y,n.z)*eps;ra.d=o;Spectrum<T>li=render1(&ra,dep+1);return li*ft;}}

    if(tp==1){n=vec3<T,V>(0,1,0);int ck=(int(floor(p.x))+int(floor(p.z)));rho=ck&1?Spectrum<T>(0.2,0.2,0.2):Spectrum<T>(0.8,0.8,0.8);}

    //printf("o(%f,%f,%f) d:(%f %f %f) h1=%f h2=%f tp=%d\n",r->o.x,r->o.y,r->o.z,r->d.x,r->d.y,r->d.z,h1,h2,tp);

    T Gi=T(1);

    T u=local.get1d(),v=local.get1d();vec3<T,V>rd=u*e1<T>+v*e2<T>;vec3<T,P>pt=sun<T>+vec3<T,P>(rd.x,rd.y,rd.z);

    vec3<T,V>dir=pt-p;vec3<T,V>n1=nor(cs(e1<T>,e2<T>));T s=len(cs(e1<T>,e2<T>));T dis=max(len(dir),eps);dir=nor(dir);Spectrum<T>le(0,0,0);

    T s1=dot(dir,n);T s2=dot(dir,n1);if(s1>T(0)&&s2>T(0)){Gi=s1*s2/(dis*dis);le=rho/T(3.14159265)*Gi*sc<T>*s;}

    ray<T>r1;vec3<T,V>nw=n*eps;r1.o=p+vec3<T,P>(nw.x,nw.y,nw.z);r1.d=sp(n,local);Spectrum<T>li=render1(&r1,dep+1);

    return rho*li+le;

},eps);assert(!isnan(a));if(a<T(1.6)){T val=(T(3.535)*a+T(2.181)*a*a)/(T(1)+T(2.276)*a+T(2.577)*a*a);
assert(!isnan(val));return val;}else return T(1);}
template<typename T>
T D(const vec3<T,V>&n,const vec3<T,V>&m){
T cosm=dot(n,m);if(cosm<=T(0))return T(0);T tanm=(1-cosm*cosm)/(cosm*cosm);
T x=X(cosm);T d=x/(T(3.1415926535)*aph<T>*aph<T>*cosm*cosm*cosm*cosm)*exp((-tanm)/(aph<T>*aph<T>));assert(!isnan(d));return d;}
template<typename T>
T F(const vec3<T,V>&ii,const vec3<T,V>&nn){auto i=nor(ii),n=nor(nn);Spectrum<T>L;
T c=abs(dot(i,n));T eta=etai<T>/etat<T>;T g2=(eta*eta)-T(1)+c*c;if(g2<T(0))return T(1);T g=sqrt(g2);
T a=g-c,b=g+c;T f=T(0.5)*(a*a)/(b*b)*(T(1)+((c*b-T(1))*(c*b-T(1)))/((c*a+T(1))*(c*a+T(1))));assert(!isnan(f));return f;}
//template<typename T>
//T P(const vec3<T,V>&m,const vec3<T,V>&n,Independent<T>&ind){return D(n,m,ind)*dot(n,m);}
template<typename T>
int delta(const vec3<T,V>&a,const vec3<T,V>&b){return abs(a.x-b.x)<eps&&abs(a.y-b.y)<eps&&abs(a.z-b.z)<eps;}
template<typename T>
vec3<T,V>refract(const vec3<T,V>&i,const vec3<T,V>&n){T eta=etai<T>/etat<T>;assert(!isnan(eta));T cosi=dot(i,n);
if(cosi<T(0)){T k=T(1)-eta*eta*(T(1)-cosi*cosi);if(k<T(0))return vec3<T,V>(0,0,0);return i*eta-(eta*cosi+sqrt(max(k,eps)))*n;}
else{eta=T(1)/eta;assert(!isnan(eta));vec3<T,V>nn=-n;cosi=-cosi;T k=T(1)-eta*eta*(T(1)-cosi*cosi);if(k<T(0))return vec3<T,V>(0,0,0);return i*eta-(eta*cosi+sqrt(max(k,eps)))*nn;}}
template<typename T>
Spectrum<T>render1(const ray<T>*r,int dep){
if(dep>100)return Spectrum<T>(0,0,0);
    T h1=hit1(r,3.0f,vec3<T,P>(0,3,0)),h2=hit2(r),t=T(1e30);int tp=-1;
    if(h1>eps&&h1<t){t=h1;tp=0;}
    if(h2>eps&&h2<t){t=h2;tp=1;}
    if(tp==-1){T u=T(0.5)*(r->d.y+T(1.0));return Spectrum<T>::lerp(Spectrum<T>(1,1,1),Spectrum<T>(0.3,0.5,0.7),u);}
    vec3<T,P>p=r->o+r->d*t;vec3<T,V>n;Spectrum<T>rho(0,0,0);
    Independent<T>local=in.clone(dep);
    if(tp==0){
        n=nor(p-vec3<T,P>(0,3,0));
        vec3<T,V>i=nor(-r->d);
        vec3<T,V>h=nor(sph(n,local));
        vec3<T,V>o=nor(reflect(r->d,h));
        T rd=local.get1d();
        T f=F(i,h);
        if(rd<f){
            T fr_a=(G(i,h,n)*G(o,h,n)*abs(dot(o,h)));
            T fr_b=max((T(4)*abs(dot(i,n))*abs(dot(h,n))),eps);
            assert(!isnan(fr_a));assert(!isnan(fr_b));
            T fr=fr_a/fr_b;
            ray<T>ra;
            ra.o=p+vec3<T,P>(n.x,n.y,n.z)*eps;
            ra.d=o;
            Spectrum<T>li=render1(&ra,dep+1);
            return li*fr;
        }
    else{
        auto v=refract(r->d,h);
        if(v.x==T(0)&&v.y==T(0)&&v.z==T(0)){
            o=nor(reflect(r->d,h));
            T fr_a=(G(i,h,n)*G(o,h,n)*abs(dot(o,h)));
            T fr_b=max((T(4)*abs(dot(i,n))*abs(dot(h,n))),eps);
            assert(!isnan(fr_a));assert(!isnan(fr_b));
            T fr=fr_a/fr_b;
            ray<T>ra;
            ra.o=p+n*eps;
            ra.d=o;
            Spectrum<T>li=render1(&ra,dep+1);
            return li*fr;
        }
        vec3<T,V>o=nor(v);
        assert(!isnan(o.x)&&!isnan(o.y)&&!isnan(o.z));
        //p(h)=D(h)|h\cdot n| -> pdf=\frac{1}{4|o\cdot h|D(h)}-> \frac{f_t}{pdf}
        T ft_a=abs(dot(i,h))*etat<T>*etat<T>*G(i,h,n)*G(o,h,n);
        T ft_b=max(T(4)*abs(dot(i,h))*abs(dot(o,n))*abs(dot(h,n))*pow((etai<T>*dot(i,h)+etat<T>*dot(o,h)),T(2)),eps);
    T ft=ft_a/ft_b;//printf("%f %f %f\n",ft,ft_a,ft_b);
    assert(!isnan(ft));ray<T>ra;ra.o=p+vec3<T,V>(n.x,n.y,n.z)*eps;ra.d=o;Spectrum<T>li=render1(&ra,dep+1);return li*ft;}}
    if(tp==1){n=vec3<T,V>(0,1,0);int ck=(int(floor(p.x))+int(floor(p.z)));rho=ck&1?Spectrum<T>(0.2,0.2,0.2):Spectrum<T>(0.8,0.8,0.8);}
    //printf("o(%f,%f,%f) d:(%f %f %f) h1=%f h2=%f tp=%d\n",r->o.x,r->o.y,r->o.z,r->d.x,r->d.y,r->d.z,h1,h2,tp);
    T Gi=T(1);
    T u=local.get1d(),v=local.get1d();vec3<T,V>rd=u*e1<T>+v*e2<T>;vec3<T,P>pt=sun<T>+vec3<T,P>(rd.x,rd.y,rd.z);
    vec3<T,V>dir=pt-p;vec3<T,V>n1=nor(cs(e1<T>,e2<T>));T s=len(cs(e1<T>,e2<T>));T dis=max(len(dir),eps);dir=nor(dir);Spectrum<T>le(0,0,0);
    T s1=dot(dir,n);T s2=dot(dir,n1);if(s1>T(0)&&s2>T(0)){Gi=s1*s2/(dis*dis);le=rho/T(3.14159265)*Gi*sc<T>*s;}
    ray<T>r1;vec3<T,V>nw=n*eps;r1.o=p+vec3<T,P>(nw.x,nw.y,nw.z);r1.d=sp(n,local);Spectrum<T>li=render1(&r1,dep+1);
    return rho*li+le;
}
template<typename T>
Spectrum<T>render2(const ray<T>*r,int dep){
if(dep>100)return Spectrum<T>(0,0,0);
    T h1=hit1(r,3.0f,vec3<T,P>(0,3,0)),h2=hit2(r),t=T(1e30);int tp=-1;
    if(h1>eps&&h1<t){t=h1;tp=0;}
    if(h2>eps&&h2<t){t=h2;tp=1;}
    if(tp==-1){T u=T(0.5)*(r->d.y+T(1.0));return Spectrum<T>::lerp(Spectrum<T>(1,1,1),Spectrum<T>(0.3,0.5,0.7),u);}
    vec3<T,P>p=r->o+r->d*t;vec3<T,V>n;Spectrum<T>rho(0,0,0);
    Independent<T>local=in.clone(dep);
    if(tp==0){n=nor(p-vec3<T,P>(0,3,0));vec3<T,V>i=nor(-r->d);vec3<T,V>h=nor(sph(n,local));vec3<T,V>o=nor(reflect(r->d,h));
    T rd=local.get1d();T f=F(i,h);T fr_a=(f*G(i,h,n)*G(o,h,n)*D(n,h));T fr_b=max((T(4)*abs(dot(i,n))*abs(dot(o,n))),eps);
    assert(!isnan(fr_a));assert(!isnan(fr_b));T fr=fr_a/fr_b;ray<T>ra;ra.o=p+vec3<T,P>(n.x,n.y,n.z)*eps;ra.d=o;
    Spectrum<T>li=render2(&ra,dep+1);return li*fr;}
    if(tp==1){n=vec3<T,V>(0,1,0);int ck=(int(floor(p.x))+int(floor(p.z)));rho=ck&1?Spectrum<T>(0.2,0.2,0.2):Spectrum<T>(0.8,0.8,0.8);}
    //printf("o(%f,%f,%f) d:(%f %f %f) h1=%f h2=%f tp=%d\n",r->o.x,r->o.y,r->o.z,r->d.x,r->d.y,r->d.z,h1,h2,tp);
    T Gi=T(1);
    T u=local.get1d(),v=local.get1d();vec3<T,V>rd=u*e1<T>+v*e2<T>;vec3<T,P>pt=sun<T>+vec3<T,P>(rd.x,rd.y,rd.z);
    vec3<T,V>dir=pt-p;vec3<T,V>n1=nor(cs(e1<T>,e2<T>));T s=len(cs(e1<T>,e2<T>));T dis=max(len(dir),eps);dir=nor(dir);Spectrum<T>le(0,0,0);
    T s1=dot(dir,n);T s2=dot(dir,n1);if(s1>T(0)&&s2>T(0)){Gi=s1*s2/(dis*dis);le=rho/T(3.14159265)*Gi*sc<T>*s;}
    ray<T>r1;vec3<T,V>nw=n*eps;r1.o=p+vec3<T,P>(nw.x,nw.y,nw.z);r1.d=sp(n,local);Spectrum<T>li=render2(&r1,dep+1);
    return rho*li+le;
}
int main(){
//printf("f:(%f %f %f) r:(%f %f %f) u:(%f %f %f)\n",cam.pos.f.x,cam.pos.f.y,cam.pos.f.z,cam.pos.r.x,cam.pos.r.y,cam.pos.r.z,cam.pos.u.x,cam.pos.u.y,cam.pos.u.z);
Projective<float,DOF<float>,Box<float>>cam(pos,lens,sh,film,w,h,45.0f);
cam.pos.mv(vec3<float,P>(0,5,20));
cam.pos=cam.pos.look(cam.pos.p,vec3<float,P>(0,3,0),vec3<float,V>(0,1,0));
//printf("f:(%f %f %f) r:(%f %f %f) u:(%f %f %f)\n",cam.pos.f.x,cam.pos.f.y,cam.pos.f.z,cam.pos.r.x,cam.pos.r.y,cam.pos.r.z,cam.pos.u.x,cam.pos.u.y,cam.pos.u.z);
int spp=1024;
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
        //printf("%f %f %f %f %f %f\n",r.o.x,r.o.y,r.o.z,r.d.z,r.d.y,r.d.z);
        Spectrum<float>L=render2(&r,2);
        //assert(!isnan(L.c[0])&&!isnan(L.c[1])&&!isnan(L.c[2]));
        //Spectrum<float>Li=rendersp(&r,1);
        sum=sum+L;//sumsp=sumsp+Li;
        }sum=sum/float(spp);//sumsp=sumsp/float(spp);
        //printf("%f %f %f\n",sum.c[0],sum.c[1],sum.c[2]);
        tile.addSample(f,vec2<float,P>(x,y),sum,1.0f);
        //tile.addSplat(vec2<T,P>(x,y),sumsp,1.0f);
    }
    film.mergeTile(tile);
}
film.writeImage("d.ppm");
return 0;
}
