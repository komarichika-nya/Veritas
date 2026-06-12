template<typename T>
HD vec3<T,P>Transform<T>::point(const vec3<T,P>&p)const{vec3<T,P>ans;
ans.x=p.x*m[0][0]+p.y*m[0][1]+p.z*m[0][2]+m[0][3];
ans.y=p.x*m[1][0]+p.y*m[1][1]+p.z*m[1][2]+m[1][3];
ans.z=p.x*m[2][0]+p.y*m[2][1]+p.z*m[2][2]+m[2][3];
T w=p.x*m[3][0]+p.y*m[3][1]+p.z*m[3][2]+m[3][3];return ans/w;}
template<typename T>
HD vec3<T,P>Transform<T>::inv_point(const vec3<T,P>&p)const{vec3<T,P>ans;
ans.x=p.x*minv[0][0]+p.y*minv[0][1]+p.z*minv[0][2]+minv[0][3];
ans.y=p.x*minv[1][0]+p.y*minv[1][1]+p.z*minv[1][2]+minv[1][3];
ans.z=p.x*minv[2][0]+p.y*minv[2][1]+p.z*minv[2][2]+minv[2][3];
T w=p.x*minv[3][0]+p.y*minv[3][1]+p.z*minv[3][2]+minv[3][3];return ans/w;}
template<typename T>
HD vec3<T,V>Transform<T>::direct(const vec3<T,V>&v)const{vec3<T,V>ans;
ans.x=v.x*m[0][0]+v.y*m[0][1]+v.z*m[0][2];
ans.y=v.x*m[1][0]+v.y*m[1][1]+v.z*m[1][2];
ans.z=v.x*m[2][0]+v.y*m[2][1]+v.z*m[2][2];return ans;}
template<typename T>
HD vec3<T,V>Transform<T>::inv_direct(const vec3<T,V>&v)const{vec3<T,V>ans;
ans.x=v.x*minv[0][0]+v.y*minv[0][1]+v.z*minv[0][2];
ans.y=v.x*minv[1][0]+v.y*minv[1][1]+v.z*minv[1][2];
ans.z=v.x*minv[2][0]+v.y*minv[2][1]+v.z*minv[2][2];return ans;}
template<typename T>
HD vec3<T,V>Transform<T>::normal(const vec3<T,V>&n)const{vec3<T,V>ans;
ans.x=minv[0][0]*n.x+minv[1][0]*n.y+minv[2][0]*n.z;
ans.y=minv[0][1]*n.x+minv[1][1]*n.y+minv[2][1]*n.z;
ans.z=minv[0][2]*n.x+minv[1][2]*n.y+minv[2][2]*n.z;return ans;}
//template<typename T>
//HD ray<<T>apply(const ray<T>&r)const{}

