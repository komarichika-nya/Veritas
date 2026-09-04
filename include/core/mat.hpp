#pragma once
#include"core/vec.hpp"
#include"core/quat.hpp"
#include"conf/conf.hpp"
#include"conf/optional.hpp"
namespace veritas{
    template<typename T>struct quat;
    template<typename T>struct mat34{
        T mat[3][4];
        mat34<T>unit(){mat34<T>v;v.mat[0][0]=v.mat[1][1]=v.mat[2][2]=1;return v;}
        mat34<T>mv(const vec3<T,V>&v){mat34<T>m;m.mat[0][3]=v.x;m.mat[1][3]=v.y;m.mat[2][3]=v.z;return m;}
        mat34<T>scale(const vec3<T,V>&v){mat34<T>m;m.mat[0][0]=v.x;m.mat[1][1]=v.y;m.mat[2][2]=v.z;return m;}
        mat34<T>rot(const quat<T>&b){mat34<T>v;quat a=quat<T>::norm(b);
            v.mat[0][0]=1-2*a.y*a.y-2*a.z*a.z;v.mat[0][1]=2*a.x*a.y-2*a.z*a.w;v.mat[0][2]=2*a.x*a.z+2*a.y*a.w;
            v.mat[1][0]=2*a.x*a.y+2*a.z*a.w;v.mat[1][1]=1-2*a.x*a.x-2*a.z*a.z;v.mat[1][2]=2*a.y*a.z-2*a.x*a.w;
            v.mat[2][0]=2*a.x*a.z-2*a.y*a.w;v.mat[2][1]=2*a.y*a.z+2*a.x*a.w;v.mat[2][2]=1-2*a.x*a.x-2*a.y*a.y;return v;}
        mat34<T>trs(const vec3<T,V>&t,const quat<T>&r,const vec3<T,V>&s){mat34 S=scale(s),R=rot(r),T1=mv(t);
// int x,y;for(x=0;x<3;x++){for(y=0;y<4;y++)printf("%f ",R.mat[x][y]);printf("\n");}
        return T1*R*S;
        }
        mat34<T>bas(const vec3<T,V>&x,const vec3<T,V>&y,const vec3<T,V>&z,const vec3<T,V>&wd){mat34 m;
            m.mat[0][0]=x.x;m.mat[0][1]=y.x;m.mat[0][2]=z.x;m.mat[0][3]=wd.x;
            m.mat[1][0]=x.y;m.mat[1][1]=y.y;m.mat[1][2]=z.y;m.mat[1][3]=wd.y;
            m.mat[2][0]=x.z;m.mat[2][1]=y.z;m.mat[2][2]=z.z;m.mat[2][3]=wd.z;return m;}
        mat34<T>operator*(const mat34<T>&v)const{
            int x,y;mat34 ans;for(x=0;x<3;x++)for(y=0;y<3;y++){
            ans.mat[x][y]=mat[x][0]*v.mat[0][y]+mat[x][1]*v.mat[1][y]+mat[x][2]*v.mat[2][y];}
            for(x=0;x<3;x++)ans.mat[x][3]=mat[x][0]*v.mat[0][3]+mat[x][1]*v.mat[1][3]+mat[x][2]*v.mat[2][3]+mat[x][3];return ans;}
        mat34<T>&operator*=(const mat34<T>&v){*this=(*this)*v;return *this;}
        vec3<T,P>operator*(const vec3<T,P>&v)const{
            return vec3<T,P>(mat[0][0]*v.x+mat[0][1]*v.y+mat[0][2]*v.z+mat[0][3],
            mat[1][0]*v.x+mat[1][1]*v.y+mat[1][2]*v.z+mat[1][3],mat[2][0]*v.x+mat[2][1]*v.y+mat[2][2]*v.z+mat[2][3]);}
        vec3<T,V>operator*(const vec3<T,V>&v)const{
            return vec3<T,V>(mat[0][0]*v.x+mat[0][1]*v.y+mat[0][2]*v.z,
            mat[1][0]*v.x+mat[1][1]*v.y+mat[1][2]*v.z,mat[2][0]*v.x+mat[2][1]*v.y+mat[2][2]*v.z);}
        mat34<T>inv(const mat34<T>&out){T a00=out.mat[0][0],a01=out.mat[0][1],a02=out.mat[0][2],a03=out.mat[0][3];
            T a10=out.mat[1][0],a11=out.mat[1][1],a12=out.mat[1][2],a13=out.mat[1][3];
            T a20=out.mat[2][0],a21=out.mat[2][1],a22=out.mat[2][2],a23=out.mat[2][3];mat34<T>r;
            T c0=a11*a22-a12*a21,c1=a10*a22-a12*a20,c2=a10*a21-a11*a20,det=a00*c0-a01*c1+a02*c2;if(!det)return unit();T d=T(1)/det;
            r.mat[0][0]=(a11*a22-a21*a12)*d;r.mat[0][1]=(a02*a21-a01*a22)*d;r.mat[0][2]=(a01*a12-a02*a11)*d;
            r.mat[1][0]=(a12*a20-a10*a22)*d;r.mat[1][1]=(a00*a22-a02*a20)*d;r.mat[1][2]=(a02*a10-a00*a12)*d;
            r.mat[2][0]=(a10*a21-a11*a20)*d;r.mat[2][1]=(a01*a20-a00*a21)*d;r.mat[2][2]=(a00*a11-a01*a10)*d;
            r.mat[0][3]=-(a03*r.mat[0][0]+a13*r.mat[0][1]+a23*r.mat[0][2]);
            r.mat[1][3]=-(a03*r.mat[1][0]+a13*r.mat[1][1]+a23*r.mat[1][2]);
            r.mat[2][3]=-(a03*r.mat[2][0]+a13*r.mat[2][1]+a23*r.mat[2][2]);return r;}
        void decompose(const mat34<T>&v,vec3<T,V>&t,quat<T>&q,vec3<T,V>&s){
            t=vec3<T,V>(v.mat[0][3],v.mat[1][3],v.mat[2][3]);
            T sx=sqrt(v.mat[0][0]*v.mat[0][0]+v.mat[1][0]*v.mat[1][0]+v.mat[2][0]*v.mat[2][0]);
            T sy=sqrt(v.mat[0][1]*v.mat[0][1]+v.mat[1][1]*v.mat[1][1]+v.mat[2][1]*v.mat[2][1]);
            T sz=sqrt(v.mat[0][2]*v.mat[0][2]+v.mat[1][2]*v.mat[1][2]+v.mat[2][2]*v.mat[2][2]);
            mat34<T>r=v;s=vec3<T,V>(sx,sy,sz);
// printf("%f %f %f\n",sx,sy,sz);
            if(sx!=0)r.mat[0][0]/=sx,r.mat[1][0]/=sx,r.mat[2][0]/=sx;
            if(sy!=0)r.mat[0][1]/=sy,r.mat[1][1]/=sy,r.mat[2][1]/=sy;
            if(sz!=0)r.mat[0][2]/=sz,r.mat[1][2]/=sz,r.mat[2][2]/=sz;
            if(det(r)<T(0)){s.x=-s.x;r.mat[0][0]=-r.mat[0][0],r.mat[1][0]=-r.mat[1][0],r.mat[2][0]=-r.mat[2][0];}
            q=quat<T>::matr(r);
        }
        mat34<T>fast_inv(const mat34<T>&out){T a00=out.mat[0][0],a01=out.mat[0][1],a02=out.mat[0][2],a03=out.mat[0][3];
            T a10=out.mat[1][0],a11=out.mat[1][1],a12=out.mat[1][2],a13=out.mat[1][3];
            T a20=out.mat[2][0],a21=out.mat[2][1],a22=out.mat[2][2],a23=out.mat[2][3];mat34<T>r;
            r.mat[0][0]=a00;r.mat[0][1]=a10;r.mat[0][2]=a20;
            r.mat[1][0]=a01;r.mat[1][1]=a11;r.mat[1][2]=a21;
            r.mat[2][0]=a02;r.mat[2][1]=a12;r.mat[2][2]=a22;
            r.mat[0][3]=-(a03*r.mat[0][0]+a13*r.mat[0][1]+a23*r.mat[0][2]);
            r.mat[1][3]=-(a03*r.mat[1][0]+a13*r.mat[1][1]+a23*r.mat[1][2]);
            r.mat[2][3]=-(a03*r.mat[2][0]+a13*r.mat[2][1]+a23*r.mat[2][2]);return r;
        }
        T det(const mat34<T>&out){
            T a00=out.mat[0][0],a01=out.mat[0][1],a02=out.mat[0][2];
            T a10=out.mat[1][0],a11=out.mat[1][1],a12=out.mat[1][2];
            T a20=out.mat[2][0],a21=out.mat[2][1],a22=out.mat[2][2];mat34<T>r;
            T c0=a11*a22-a12*a21,c1=a10*a22-a12*a20,c2=a10*a21-a11*a20,d=a00*c0-a01*c1+a02*c2;return d;}
        void to_3x3(T(&v)[3][3])const{int x,y;for(x=0;x<3;x++)for(y=0;y<3;y++)v[x][y]=mat[x][y];}
    };
    template<typename T,int N>struct mat{
        T m[N][N];
        mat()=default;
        mat<T,N>unit(){mat<T,N>v;int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)if(x==y)v.m[x][y]=T(1);else v.m[x][y]=T(0);return v;}
        mat<T,N>zero(){mat<T,N>v;int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)v.m[x][y]=T(0);return v;}
        T det()const requires(N==3){
        T a=m[0][0]*(m[1][1]*m[2][2]-m[1][2]*m[2][1])-m[0][1]*(m[1][0]*m[2][2]-m[1][2]*m[2][0])+m[0][2]*(m[1][0]*m[2][1]-m[1][1]*m[2][0]);
        return a;
    }
//trust me,I will add,but not now.
        T det()const requires(N==4){
            T a=m[0][0]*(m[1][1]*(m[2][2]*m[3][3]-m[2][3]*m[3][2])-m[1][2]*(m[2][1]*m[3][3]-m[2][3]*m[3][1])+m[1][3]*(m[2][1]*m[3][2]-m[2][2]*m[3][1]));
            T b=m[0][1]*(m[1][0]*(m[2][2]*m[3][3]-m[2][3]*m[3][2])-m[1][2]*(m[2][0]*m[3][3]-m[2][3]*m[3][0])+m[1][3]*(m[2][0]*m[3][2]-m[2][2]*m[3][0]));
            T c=m[0][2]*(m[1][0]*(m[2][1]*m[3][3]-m[2][3]*m[3][1])-m[1][1]*(m[2][0]*m[3][3]-m[2][3]*m[3][0])+m[1][3]*(m[2][0]*m[3][1]-m[2][1]*m[3][0]));
            T d=m[0][3]*(m[1][0]*(m[2][1]*m[3][2]-m[2][2]*m[3][1])-m[1][1]*(m[2][0]*m[3][2]-m[2][2]*m[3][0])+m[1][2]*(m[2][0]*m[3][1]-m[2][1]*m[3][0]));
            return a-b+c-d;
        }
        mat<T,N>operator+(const mat<T,N>&v)const{mat<T,N>a;int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)a.m[x][y]=m[x][y]+v.m[x][y];return a;}
        vec<T,V,N>operator*(const vec<T,V,N>&v)const{vec<T,V,N>a;int x,y;for(x=0;x<N;x++){a[x]=T(0);for(y=0;y<N;y++)a[x]+=m[x][y]*v[y];}return a;}
        mat<T,N>operator*(const mat<T,N>&v)const{mat<T,N>a;int x,y,z;for(x=0;x<N;x++)for(y=0;y<N;y++)for(z=0;z<N;z++)a.m[x][y]+=m[x][z]*v.m[z][y];return a;}
        mat<T,N>operator*(T s)const{mat<T,N>a;int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)a.m[x][y]=m[x][y]*s;return a;}
        mat<T,N>operator/(T s)const{mat<T,N>a;if(s==T(0))return mat<T,N>::zero();T s1=T(1)/s;int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)a.m[x][y]=m[x][y]*s1;return a;}
        veritas::fsytd::optional<mat<T,N>>inv(const mat<T,N>&v)requires(N==3){
            T a=v.m[0][0],b=v.m[0][1],c=v.m[0][2];
            T d=v.m[1][0],e=v.m[1][1],f=v.m[1][2];
            T g=v.m[2][0],h=v.m[2][1],i=v.m[2][2];
            T c00=e*i-f*h,c01=f*g-d*i,c02=d*h-e*g;
            T c10=c*h-b*i,c11=a*i-c*g,c12=b*g-a*h;
            T c20=b*f-c*e,c21=c*d-a*f,c22=a*e-b*d;
            T det=a*c00+b*c01+c*c02;if(det==T(0))return{};
            T invd=T(1)/det;mat<T,N>ans;
            ans.m[0][0]=c00*invd;ans.m[1][0]=c01*invd;ans.m[2][0]=c02*invd;
            ans.m[0][1]=c10*invd;ans.m[1][1]=c11*invd;ans.m[2][1]=c12*invd;
            ans.m[0][2]=c20*invd;ans.m[1][2]=c21*invd;ans.m[2][2]=c22*invd;return ans;
        }
//I'm scared with this...
        veritas::fsytd::optional<mat<T,N>>inv(const mat<T,N>&v)requires(N==4){
            auto d3=[&](T a00,T a01,T a02,T a10,T a11,T a12,T a20,T a21,T a22)->T{return a00*(a11*a22-a12*a21)-a01*(a10*a22-a12*a20)+a02*(a10*a21-a11*a20);};
            const T(&a)[4][4]=v.m;T det,tp[4][4];
            tp[0][0]=d3(a[1][1],a[1][2],a[1][3],a[2][1],a[2][2],a[2][3],a[3][1],a[3][2],a[3][3]);
            tp[1][0]=-d3(a[0][1],a[0][2],a[0][3],a[2][1],a[2][2],a[2][3],a[3][1],a[3][2],a[3][3]);
            tp[2][0]=d3(a[0][1],a[0][2],a[0][3],a[1][1],a[1][2],a[1][3],a[3][1],a[3][2],a[3][3]);
            tp[3][0]=-d3(a[0][1],a[0][2],a[0][3],a[1][1],a[1][2],a[1][3],a[2][1],a[2][2],a[2][3]);
            tp[0][1]=-d3(a[1][0],a[1][2],a[1][3],a[2][0],a[2][2],a[2][3],a[3][0],a[3][2],a[3][3]);
            tp[1][1]=d3(a[0][0],a[0][2],a[0][3],a[2][0],a[2][2],a[2][3],a[3][0],a[3][2],a[3][3]);
            tp[2][1]=-d3(a[0][0],a[0][2],a[0][3],a[1][0],a[1][2],a[1][3],a[3][0],a[3][2],a[3][3]);
            tp[3][1]=d3(a[0][0],a[0][2],a[0][3],a[1][0],a[1][2],a[1][3],a[2][0],a[2][2],a[2][3]);
            tp[0][2]=d3(a[1][0],a[1][1],a[1][3],a[2][0],a[2][1],a[2][3],a[3][0],a[3][1],a[3][3]);
            tp[1][2]=-d3(a[0][0],a[0][1],a[0][3],a[2][0],a[2][1],a[2][3],a[3][0],a[3][1],a[3][3]);
            tp[2][2]=d3(a[0][0],a[0][1],a[0][3],a[1][0],a[1][1],a[1][3],a[3][0],a[3][1],a[3][3]);
            tp[3][2]=-d3(a[0][0],a[0][1],a[0][3],a[1][0],a[1][1],a[1][3],a[2][0],a[2][1],a[2][3]);
            tp[0][3]=-d3(a[1][0],a[1][1],a[1][2],a[2][0],a[2][1],a[2][2],a[3][0],a[3][1],a[3][2]);
            tp[1][3]=d3(a[0][0],a[0][1],a[0][2],a[2][0],a[2][1],a[2][2],a[3][0],a[3][1],a[3][2]);
            tp[2][3]=-d3(a[0][0],a[0][1],a[0][2],a[1][0],a[1][1],a[1][2],a[3][0],a[3][1],a[3][2]);
            tp[3][3]=d3(a[0][0],a[0][1],a[0][2],a[1][0],a[1][1],a[1][2],a[2][0],a[2][1],a[2][2]);
            det=a[0][0]*tp[0][0]+a[0][1]*tp[0][1]+a[0][2]*tp[0][2]+a[0][3]*tp[0][3];if(det==T(0))return{};
            T invd=T(1)/det;mat<T,N>ans;int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)ans.m[x][y]=tp[y][x]*invd;return ans;
        }
    };
//add more btw.
    template<typename T>
    using mat4=mat<T,4>;
}//namespace veritas
