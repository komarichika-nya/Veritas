template<typename T>
const Shape<Triangle,T>*Mesh<T>::getMesh()const{return(*all)[mesh_idx];}
template<typename T>
T Mesh<T>::area()const{
    auto*mesh=getMesh();const int*v=&mesh->idx_vec[tri_idx*3];
    vec3<T,P>p0=mesh->pos[v[0]],p1=mesh->pos[v[1]],p2=mesh->pos[v[2]];
    return T(0.5)*len(cs(p1-p0,p2-p0));
}
template<typename T>
HD T area(const vec3<T,P>&p0,const vec3<T,P>&p1,const vec3<T,P>&p2)const{
    return T(0.5)*len(cs(p1-p0,p2-p0));
}
template<typename T>
HD bound3<T>bounds(const vec3<T,P>&p0,const vec3<T,P>&p1,const vec3<T,P>&p2){
    return bound3<T>::add(bound3<T>(p0,p1),p2);
}
template<typename T>
HD T solid_angle(const vec3<T,P>&p0,const vec3<T,P>&p1,const vec3<T,P>&p2,const vec3<T,P>&p){
    const auto*mesh=getMesh();const int*v=&mesh->idx_vec[tri_idx*3];
    vec3<T,P>p0=mesh->pos[v[0]],p1=mesh->pos[v[1]],p2=mesh->pos[v[2]];
    return spherical_area(nor(p0-p),nor(p1-p),nor(p2-p));
}
template<typename T>
HD fsytd::optional<hit<T>>intersect(const ray<T>*r,const vec3<T,P>&p0,const vec3<T,P>&p1,const vec3<T,P>&p2){
    vec3<T,V>e1=p1-p0,e2=p2-p0,pv=cs(r->d,e2);T det=dot(e1,pv);
    if(abs(det)<eps)return{};T inv=T(1)/det;
    vec3<T,V>tv=r->o-p0,T u=dot(tv,pv)*inv;if(u<T(0)||u>T(1))return{};
    vec3<T,V>qv=cs(tv,e1);T v=dot(r->d,qv)*inv;if(v<T(0)||u+v>T(1))return{};
    T t=dot(e2,qv)*inv;if(t<r->tmn||t>r->tmx)return{};return hit<T>{t,T(1)-u-v,u,v};
}


