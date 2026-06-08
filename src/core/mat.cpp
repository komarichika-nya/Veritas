#include"conf/conf.hpp"
#include"core/mat.hpp"
template<typename T>HD mat34<T>::mat34(){int x,y;for(x=0;x<3;x++)for(y=0;y<4;y++){if(x==y)mat[x][y]=1;else mat[x][y]=0;}};
template<typename T>
HD mat34<T>mat34<T>::unit(){mat34<T>v;v.mat[0][0]=v.mat[1][1]=v.mat[2][2]=1;return v;}
template<typename T>
HD mat34<T>mat34<T>::mv(const vec3<T,V>&v){mat34<T>m;m.mat[0][3]=v.x;m.mat[1][3]=v.y;m.mat[2][3]=v.z;return m;}
template<typename T>
HD mat34<T>mat34<T>::scale(const vec3<T,V>&v){mat34<T>m;m.mat[0][0]=v.x;m.mat[1][1]=v.y;m.mat[2][2]=v.z;return m;}
template<typename T>
HD mat34<T>mat34<T>::rot(const quat<T>&b){mat34<T>v;quat a=quat<T>::norm(b);
v.mat[0][0]=1-2*a.y*a.y-2*a.z*a.z;v.mat[0][1]=2*a.x*a.y-2*a.z*a.w;v.mat[0][2]=2*a.x*a.z+2*a.y*a.w;
v.mat[1][0]=2*a.x*a.y+2*a.z*a.w;v.mat[1][1]=1-2*a.x*a.x-2*a.z*a.z;v.mat[1][2]=2*a.y*a.z-2*a.x*a.w;
v.mat[2][0]=2*a.x*a.z-2*a.y*a.w;v.mat[2][1]=2*a.y*a.z+2*a.x*a.w;v.mat[2][2]=1-2*a.x*a.x-2*a.y*a.y;return v;}
template<typename T>
HD mat34<T>mat34<T>::trs(const vec3<T,V>&t,const quat<T>&r,const vec3<T,V>&s){mat34 S=scale(s),R=rot(r),T1=mv(t);
// int x,y;for(x=0;x<3;x++){for(y=0;y<4;y++)printf("%f ",R.mat[x][y]);printf("\n");}
return T1*R*S;}
template<typename T>
HD mat34<T>mat34<T>::bas(const vec3<T,V>&x,const vec3<T,V>&y,const vec3<T,V>&z,const vec3<T,V>&wd){mat34 m;
m.mat[0][0]=x.x;m.mat[0][1]=y.x;m.mat[0][2]=z.x;m.mat[0][3]=wd.x;
m.mat[1][0]=x.y;m.mat[1][1]=y.y;m.mat[1][2]=z.y;m.mat[1][3]=wd.y;
m.mat[2][0]=x.z;m.mat[2][1]=y.z;m.mat[2][2]=z.z;m.mat[2][3]=wd.z;return m;}
template<typename T>
HD mat34<T>mat34<T>::operator*(const mat34<T>&v)const{
int x,y;mat34 ans;for(x=0;x<3;x++)for(y=0;y<3;y++){
ans.mat[x][y]=mat[x][0]*v.mat[0][y]+mat[x][1]*v.mat[1][y]+mat[x][2]*v.mat[2][y];}
for(x=0;x<3;x++)ans.mat[x][3]=mat[x][0]*v.mat[0][3]+mat[x][1]*v.mat[1][3]+mat[x][2]*v.mat[2][3]+mat[x][3];return ans;}
template<typename T>
HD mat34<T>&mat34<T>::operator*=(const mat34<T>&v){*this=(*this)*v;return *this;}
template<typename T>
HD vec3<T,P>mat34<T>::operator*(const vec3<T,P>&v)const{
return vec3<T,P>(mat[0][0]*v.x+mat[0][1]*v.y+mat[0][2]*v.z+mat[0][3],
mat[1][0]*v.x+mat[1][1]*v.y+mat[1][2]*v.z+mat[1][3],mat[2][0]*v.x+mat[2][1]*v.y+mat[2][2]*v.z+mat[2][3]);}
template<typename T>   
HD vec3<T,V>mat34<T>::operator*(const vec3<T,V>&v)const{
return vec3<T,V>(mat[0][0]*v.x+mat[0][1]*v.y+mat[0][2]*v.z,
mat[1][0]*v.x+mat[1][1]*v.y+mat[1][2]*v.z,mat[2][0]*v.x+mat[2][1]*v.y+mat[2][2]*v.z);}
template<typename T>    
HD mat34<T>mat34<T>::inv(const mat34<T>&out){T a00=out.mat[0][0],a01=out.mat[0][1],a02=out.mat[0][2],a03=out.mat[0][3];
T a10=out.mat[1][0],a11=out.mat[1][1],a12=out.mat[1][2],a13=out.mat[1][3];
T a20=out.mat[2][0],a21=out.mat[2][1],a22=out.mat[2][2],a23=out.mat[2][3];mat34<T>r;
T c0=a11*a22-a12*a21,c1=a10*a22-a12*a20,c2=a10*a21-a11*a20,det=a00*c0-a01*c1+a02*c2;if(!det)return unit();T d=T(1)/det;
r.mat[0][0]=(a11*a22-a21*a12)*d;r.mat[0][1]=(a02*a21-a01*a22)*d;r.mat[0][2]=(a01*a12-a02*a11)*d;
r.mat[1][0]=(a12*a20-a10*a22)*d;r.mat[1][1]=(a00*a22-a02*a20)*d;r.mat[1][2]=(a02*a10-a00*a12)*d;
r.mat[2][0]=(a10*a21-a11*a20)*d;r.mat[2][1]=(a01*a20-a00*a21)*d;r.mat[2][2]=(a00*a11-a01*a10)*d;
r.mat[0][3]=-(a03*r.mat[0][0]+a13*r.mat[0][1]+a23*r.mat[0][2]);
r.mat[1][3]=-(a03*r.mat[1][0]+a13*r.mat[1][1]+a23*r.mat[1][2]);
r.mat[2][3]=-(a03*r.mat[2][0]+a13*r.mat[2][1]+a23*r.mat[2][2]);return r;}
template<typename T>    
HD void mat34<T>::decompose(const mat34<T>&v,vec3<T,V>&t,quat<T>&q,vec3<T,V>&s){
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
q=quat<T>::matr(r);}
template<typename T>
HD mat34<T>mat34<T>::fast_inv(const mat34<T>&out){T a00=out.mat[0][0],a01=out.mat[0][1],a02=out.mat[0][2],a03=out.mat[0][3];
T a10=out.mat[1][0],a11=out.mat[1][1],a12=out.mat[1][2],a13=out.mat[1][3];
T a20=out.mat[2][0],a21=out.mat[2][1],a22=out.mat[2][2],a23=out.mat[2][3];mat34<T>r;
r.mat[0][0]=a00;r.mat[0][1]=a10;r.mat[0][2]=a20;
r.mat[1][0]=a01;r.mat[1][1]=a11;r.mat[1][2]=a21;
r.mat[2][0]=a02;r.mat[2][1]=a12;r.mat[2][2]=a22;
r.mat[0][3]=-(a03*r.mat[0][0]+a13*r.mat[0][1]+a23*r.mat[0][2]);
r.mat[1][3]=-(a03*r.mat[1][0]+a13*r.mat[1][1]+a23*r.mat[1][2]);
r.mat[2][3]=-(a03*r.mat[2][0]+a13*r.mat[2][1]+a23*r.mat[2][2]);return r;}
template<typename T>
HD T mat34<T>::det(const mat34<T>&out){
T a00=out.mat[0][0],a01=out.mat[0][1],a02=out.mat[0][2];
T a10=out.mat[1][0],a11=out.mat[1][1],a12=out.mat[1][2];
T a20=out.mat[2][0],a21=out.mat[2][1],a22=out.mat[2][2];mat34<T>r;
T c0=a11*a22-a12*a21,c1=a10*a22-a12*a20,c2=a10*a21-a11*a20,d=a00*c0-a01*c1+a02*c2;return d;}
template<typename T>
HD void mat34<T>::to_3x3(T(&v)[3][3])const{int x,y;for(x=0;x<3;x++)for(y=0;y<3;y++)v[x][y]=mat[x][y];}
//square matrix
template<typename T,int N>
HD mat<T>::mat()=default;
template<typename T,int N>
HD mat<T,N>mat<T,N>::unit(){mat<T,N>v;int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)v.m[x][y]=T(1);return v;}
template<typename T,int N>
HD mat<T,N>mat<T,N>::zero(){mat<T,N>v;int x,y;for(x=0;x<N;x++)for(y=0;x<N;y++)v.m[x][y]=T(0);return v;}
template<typename T,int N>
HD T mat<T,3>::det()const requires(N==3){
T a=m[0][0]*m[1][1]*m[2][2]+m[0][1]*m[1][2]*m[2][1]+m[0][2]*m[0][1]*m[2][1];
T b=m[0][2]*m[1][1]*m[2][2]+m[0][1]*m[1][0]*m[2][2]+m[0][0]*m[1][2]*m[2][1];
return a-b;}
//trust me,I will add,but not now.
template<typename T,int N>
HD T mat<T,N>::det()const requires(N==4){
T a=m[0][0]*(m[1][1]*(m[2][2]*m[3][3]-m[2][3]*m[3][2])-m[1][2]*(m[2][1]*m[3][3]-m[2][3]*m[3][1])+m[1][3]*(m[2][1]*m[3][2]-m[2][2]*m[3][1]));
T b=m[0][1]*(m[1][0]*(m[2][2]*m[3][3]-m[2][3]*m[3][2])-m[1][2]*(m[2][0]*m[3][3]-m[2][3]*m[3][0])+m[1][3]*(m[2][0]*m[3][2]-m[2][2]*m[3][0]));
T c=m[0][2]*(m[1][0]*(m[2][1]*m[3][3]-m[2][3]*m[3][1])-m[1][1]*(m[2][0]*m[3][3]-m[2][3]*m[3][0])+m[1][3]*(m[2][0]*m[3][1]-m[2][1]*m[3][0]));
T d=m[0][3]*(m[1][0]*(m[2][1]*m[3][2]-m[2][2]*m[3][1])-m[1][1]*(m[2][0]*m[3][2]-m[2][2]*m[3][0])+m[1][2]*(m[2][0]*m[3][1]-m[2][1]*m[3][0]));
return a-b+c-d;}
template<typename T,int N>
HD mat<T,N>mat<T,N>::operator+(const mat<T,N>&v)const{mat<T,N>a;int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)a.m[x][y]=m[x][y]+v.m[x][y];return a;}
template<typename T,int N>
HD vec<T,V,M>mat<T,N>::operator*(const vec<T,V,N>&v)const{vec<T,V,N>a;int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)a[x]+=m[x][y]*v[x];return a;}
template<typename T,int N>
HD mat<T,N>mat<T,N>::operator*(const mat<T,N>&v)const{mat<T,N>a;int x,y,k;for(x=0;x<N;x++)for(y=0;y<N;y++)for(z=0;z<N;z++)a.m[x][y]=m[x][z]*v.m[z][y];return a;}
template<typename T,int N>
HD mat<T,N>mat<T,N>::operator*(T s)const{mat<T,N>a;int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)a.m[x][y]=m[x][y]*s;return a;}
template<typename T,int N>
HD mat<T,N>mat<T,N>::operator/(T s)const{mat<T,N>a;s=max(s,eps);int x,y;for(x=0;x<N;x++)for(y=0;y<N;y++)a.m[x][y]=m[x][y]/s;return a;}
template<typename T,int N>
HD mat<T,N>mat<T,3>::inv()requires(N==3){}
template<typename T,int N>
HD mat<T,N>mat<T,4>::inv()requires(N==4){}
