template<typename T>
const Shape<Triangle,T>*Mesh<T>::getMesh()const{return(*all)[mesh_idx];}
template<typename T>
HD bound3<T>bounds()const{
    auto*mesh=getMesh();const int*v=&mesh->idx_vec[tri_idx*3];
    vec3<T,P>p0=mesh->pos[v[0]],p1=mesh->pos[v[1]],p2=mesh->pos[v[2]];
    return bound3<T>::add(bound3<T>(p0,p1),p2);
}
template<typename T>
HD T Mesh<T>::area()const{
    auto*mesh=getMesh();const int*v=&mesh->idx_vec[tri_idx*3];
    vec3<T,P>p0=mesh->pos[v[0]],p1=mesh->pos[v[1]],p2=mesh->pos[v[2]];
    return T(0.5)*len(cs(p1-p0,p2-p0));
}
template<typename T>
HD T solid_angle(const vec3<T,P>&p)const{
    const auto*mesh=getMesh();const int*v=&mesh->idx_vec[tri_idx*3];
    vec3<T,P>p0=mesh->pos[v[0]],p1=->pos[v[1]],p2=pos[v[2]];
    return spherical_area(nor(p0-p),nor(p1-p),nor(p2-p));
}
template<typename T>
HD optional<hit<T>>intersect(const ray<T>*r,const vec3<T,P>&p0,const vec3<T,P>&p1,const vec3<T,P>&p2){
    if(len(cs(p1-p0,p2-p0))==T(0))return{};vec3<T,V>T=r->o-p0,s1=p1-p0,s2=p2-p0;
    T det=dot(cs(s1,s2),-d);if(det<=T(0))return{};T u=dot(cs(T,s2),-d)/det;
    if(u<T(0)||u>T(1))return{lim<T>::max(),lim<T>::max(),lim<T>::max(),lim<T>::max()};
    T v=dot(cs(s1,t),-d);if(v<T(0)||u+v>T(1))return{lim<T>::max(),lim<T>::max(),lim<T>::max(),lim<T>::max()};
    T t=dot(cs(s1,s2),T);if(t<eps)return{lim<T>::max(),lim<T>::max(),lim<T>::max(),lim<T>::max()};
    return{t,u,v,T(1)-u-v};
}


