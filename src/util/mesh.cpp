#include<cstdio>
template<typename T>
std::vector<const Shape<Triangle,T>*>*Mesh<T>::build(const Shape<Triangle,T>*s,fsytd::allocator<Shape<Triangle,T>>alloc){
    if(!v){using Vecalloc=typename fsytd::allocator<std::vector<Shape<Triangle,T>>>::template rebind<std::vector<const Shape<Triangle,T>*>>::other;
    Vecalloc vecalloc(alloc);v=vecalloc.allocate(1);vecalloc.construct(v);}if(std::find(v->begin(),v->end(),s)==v->end())v->push_back(s);
    //printf("v:%p\n",v);
    return v;
}
template<typename T>
HD const T Mesh<T>::area(size_t mesh_idx,size_t tri_idx)const{
    auto*mesh=getMesh(mesh_idx);const int*v=&mesh->idx_vec[tri_idx*3];
    vec3<T,P>p0=mesh->pos[v[0]],p1=mesh->pos[v[1]],p2=mesh->pos[v[2]];
    return T(0.5)*len(cs(p1-p0,p2-p0));
}
template<typename T>
HD const bound3<T>Mesh<T>::bounds(size_t mesh_idx,size_t tri_idx)const{
    auto*mesh=getMesh(mesh_idx);const int*v=&mesh->idx_vec[tri_idx*3];
    vec3<T,P>p0=mesh->pos[v[0]],p1=mesh->pos[v[1]],p2=mesh->pos[v[2]];
    return bound3<T>::add(bound3<T>(p0,p1),p2);
}
template<typename T>
HD const T Mesh<T>::solid_angle(size_t mesh_idx,size_t tri_idx,const vec3<T,P>&p)const{
    const auto*mesh=getMesh(mesh_idx);const int*v=&mesh->idx_vec[tri_idx*3];
    vec3<T,P>p0=mesh->pos[v[0]],p1=mesh->pos[v[1]],p2=mesh->pos[v[2]];
    return spherical_area(nor(p0-p),nor(p1-p),nor(p2-p));
}

