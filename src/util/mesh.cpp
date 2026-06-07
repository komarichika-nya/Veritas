#include<cassert>
using std::vector;
template<typename T>
Mesh<T>::Mesh()=default;
template<typename T>
Mesh<T>::Mesh(const vector<vec3<T,P>>&pos,const vector<vec3<T,V>>&face_normal,const vector<vec3<T,V>>&shading_normal,const vector<vec2<T,P>>&uvs,const vector<vec3<int,P>>&idx,int cnt):
pos(pos),face_normal(face_normal),shading_normal(shading_normal),uvs(uvs),idx(idx),count(cnt){}
template<typename T>
Mesh<T>::Mesh(const vector<vec3<T,P>>&pos,const vector<vec3<T,V>>&face_normal,const vector<vec3<int,P>>&idx):pos(pos),face_normal(face_normal),idx(idx){}
template<typename T>
Mesh<T>::Mesh(const vector<vec3<T,P>>&pos,const vector<vec3<int,P>>&idx,int cnt):pos(pos),idx(idx),count(cnt){}
template<typename T>
Mesh<T>::Mesh(const vector<vec3<T,P>>&pos,const vector<vec3<int,P>>&idx):pos(pos),idx(idx){}
template<typename T>
bound3<T>Mesh<T>::bounds()const{vec3<T,P>mx(lim<T>::min(),lim<T>::min()),mn(lim<T>::max(),lim<T>::max());
for(auto x:pos)mx=max(mx,x),mn=min(mn,x);return bound3<T>(mn,mx);}
template<typename T>
T Mesh<T>::intersect(const ray<T>*r,int p)const{assert(p<(int)idx.size());
vec3<T,P>a=pos[idx[p].x],b=pos[idx[p].y],c=pos[idx[p].z];
//printf("%f %f %f %f %f %f %f %f %f\n",a.x,a.y,a.z,b.x,b.y,b.z,c.x,c.y,c.z);
vec3<T,V>t=r->o-a,ab=b-a,ac=c-a;
T det=dot(ab,cs(ac,r->d));if(det<eps)return T(-1);T u=dot(t,cs(ac,r->d))/det;if(u<T(0)||u>T(1))return T(-1);
T v=dot(ab,cs(t,r->d))/det;if(v<T(0)||u+v>T(1))return T(-1);
T w=dot(ab,cs(ac,-t))/det;if(w<T(0))return T(-1);return w;}
template<typename T>
vec3<T,V>Mesh<T>::get_normal(int p)const{assert(p<(int)idx.size());if(!face_normal.empty())return face_normal[p];
vec3<T,P>a=pos[idx[p].x],b=pos[idx[p].y],c=pos[idx[p].z];vec3<T,V>n=cs(b-a,c-a);T l=len(n);if(l==T(0))return vec3<T,V>(0,0,0);return n/l;}
template<typename T>
T Mesh<T>::area(int p)const{assert(p<(int)idx.size());return T(0.5)*len(cs(pos[idx[p].y]-pos[idx[p].x],pos[idx[p].z]-pos[idx[p].x]));}
template<typename T>
surface<T>Mesh<T>::get(const ray<T>*r,int p)const{T dis=intersect(r,p);if(dis==T(-1))return{};surface<T>sur{};
if(!pos.empty())sur.pos=r->o+dis*r->d;if(!face_normal.empty())sur.face_normal=face_normal[p];if(!uvs.empty())sur.u=uvs[p].x,sur.v=uvs[p].y;
if(!shading_normal.empty())sur.shading_normal=shading_normal[p];return sur;}
