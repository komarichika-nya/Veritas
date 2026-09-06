#pragma once
#include<cstddef>
#include<vector>
#include<algorithm>
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"core/bound.hpp"
#include"core/ray.hpp"
#include"surface.hpp"
#include"conf/optional.hpp"
#include"conf/lim.hpp"
#include"util/shape.hpp"
#include"conf/span.hpp"
namespace veritas{
    template<typename T>class Mesh{
        const Shape<Triangle,T>*mesh=nullptr;
        //const Shape<Triangle,T>*getMesh()const{assert(mesh_idx>=0&&static_cast<size_t>(mesh_idx)<v->size());return (*v)[mesh_idx];}
    public:
        constexpr Mesh()=default;
        constexpr Mesh(const Shape<Triangle,T>*mesh):mesh(mesh){}
        const T area(int tri_idx)const{
            const int*id=&mesh->idx_vec[tri_idx*3];
            vec3<T,P>p0=mesh->pos[id[0]],p1=mesh->pos[id[1]],p2=mesh->pos[id[2]];
            return T(0.5)*len(cs(p1-p0,p2-p0));
        }
        const bound3<T>bounds(int tri_idx)const{
            const int*id=&mesh->idx_vec[tri_idx*3];
            vec3<T,P>p0=mesh->pos[id[0]],p1=mesh->pos[id[1]],p2=mesh->pos[id[2]];
            return bound3<T>::add(bound3<T>(p0,p1),p2);
        }
        const T solid_angle(int tri_idx,const vec3<T,P>&p)const{
            const int*id=&mesh->idx_vec[tri_idx*3];
            vec3<T,P>p0=mesh->pos[id[0]],p1=mesh->pos[id[1]],p2=mesh->pos[id[2]];
            return spherical_area(nor(p0-p),nor(p1-p),nor(p2-p));
        }
        veritas::fsytd::optional<surface<T>>intersect(int tri_idx,const ray<T>*r)const{
            //assert(tri_idx<66);
            const int*id=&mesh->idx_vec[tri_idx*3];
            //printf("%d %d %d %d\n",sizeof(mesh->pos)/sizeof(vec3<T,P>),id[0],id[1],id[2]);
            vec3<T,P>p0=mesh->pos[id[0]],p1=mesh->pos[id[1]],p2=mesh->pos[id[2]];
            vec3<T,V>e1=p1-p0,e2=p2-p0,pv=cs(r->d,e2);T det=dot(e1,pv);
            if(fsytd::abs(det)<veritas::fsytd::lim<T>::eps())return{};T inv=T(1)/det;
            vec3<T,V>tv=r->o-p0;T u=dot(tv,pv)*inv;if(u<T(0)||u>T(1))return{};
            vec3<T,V>qv=cs(tv,e1);T v=dot(r->d,qv)*inv;if(v<T(0)||u+v>T(1))return{};
            T t=dot(e2,qv)*inv;if(t<r->tmn||t>r->tmx)return{};surface<T>s;
            vec3<T,V>ng=nor(cs(e1,e2));if(mesh->is_reverse)ng=-ng;vec3<T,V>ns=ng;
            if(mesh->normal){vec3<T,V>n0=mesh->normal[id[0]],n1=mesh->normal[id[1]],n2=mesh->normal[id[2]];
            ns=nor(n0*(T(1)-u-v)+n1*u+n2*v);if(mesh->is_reverse)ns=-ns;if(dot(ng,ns)<T(0))ng=-ng;}
            bool back=dot(ng,r->d)>T(0);if(back)ng=-ng,ns=-ns;
            s.n=ng;s.pos=r->o+r->d*t;s.bary=vec3<T,P>(T(1-u-v),u,v);s.shading.n=ns;s.face_idx=tri_idx;s.flip=back;s.wo=-nor(r->d);s.t=t;
            if(mesh->uv)s.uv=(T(1)-u-v)*mesh->uv[id[0]]+u*mesh->uv[id[1]]+v*mesh->uv[id[2]];
            //dpu,dpv,dnu,dnv
            return s;
        }
        const Shape<Triangle,T>*getMesh()const{return mesh;}
    };
    //Sence::std::vector<Mesh<T>*>*sence;
    //for x in h:
    //  for y in w:
    //      auto check=(*sence)[mesh_idx]->intersect(tri_idx,&r)
    //      render()
    //      ...
}//namespace veritas

