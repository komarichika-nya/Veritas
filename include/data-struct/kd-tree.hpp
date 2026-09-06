//deeply love for data-struct
#pragma once
#include<vector>
#include<cstddef>
#include"conf/conf.hpp"
#include"conf/span.hpp"
#include"util/shape.hpp"
#include"core/bound.hpp"
#include"core/vec.hpp"
#include"conf/allocator.hpp"
#include"util/mesh.hpp"
#include"util/shape.hpp"
namespace veritas{
    enum SplitMode{sah=0,middle=1};
    template<typename T>class Kd_tree{
    public:
        std::vector<int>idx;
        const Shape<Triangle,T>*mesh;
        Kd_tree()=default;
        Kd_tree(const Mesh<T>&m,T ct,T ci){
            mesh=m.getMesh();
            this->ci=ci;this->ct=ct;
            idx.resize(mesh->num_idx);
            std::iota(idx.begin(),idx.end(),0);
            init();
        }
        //return the root index
        template<SplitMode mode>
        int build(int l,int r){
            if(l>r)return 0;
            int mid=0;
            if constexpr(mode==SplitMode::sah)mid=sah(l,r);
            if constexpr(mode==SplitMode::middle)mid=l+r>>1;
            ls[mid]=this->build<mode>(l,mid);
            rs[mid]=this->build<mode>(mid+1,r);
            update(mid);
            return mid;
        }
    private:
        T ci,ct;int M=12;//M is called number of mat
        int*ls=nullptr,*rs=nullptr;T*lx=nullptr,*rx=nullptr,*ly=nullptr,*ry=nullptr,*lz=nullptr,*rz=nullptr;
        void init(){
            fsytd::allocator<int>alloc_int;
            fsytd::allocator<T>alloc_T;
            ls=alloc_int.allocate(mesh->num_idx);
            rs=alloc_int.allocate(mesh->num_idx);
            lx=alloc_T.allocate(mesh->num_vec);
            rx=alloc_T.allocate(mesh->num_vec);
            ly=alloc_T.allocate(mesh->num_vec);
            ry=alloc_T.allocate(mesh->num_vec);
            lz=alloc_T.allocate(mesh->num_vec);
            rz=alloc_T.allocate(mesh->num_vec);
        }
        void update(int x){
            int q=idx[x];const int*id=&mesh->idx_vec[q*3];
            lx[x]=fsytd::min(mesh->pos[id[0]].x,fsytd::min(mesh->pos[id[1]].x,mesh->pos[id[2]].x));
            rx[x]=fsytd::max(mesh->pos[id[0]].x,fsytd::max(mesh->pos[id[1]].x,mesh->pos[id[2]].x));
            ly[x]=fsytd::min(mesh->pos[id[0]].y,fsytd::min(mesh->pos[id[1]].y,mesh->pos[id[2]].y));
            ry[x]=fsytd::max(mesh->pos[id[0]].y,fsytd::max(mesh->pos[id[1]].y,mesh->pos[id[2]].y));
            lz[x]=fsytd::min(mesh->pos[id[0]].z,fsytd::min(mesh->pos[id[1]].z,mesh->pos[id[2]].z));
            rz[x]=fsytd::max(mesh->pos[id[0]].z,fsytd::max(mesh->pos[id[1]].z,mesh->pos[id[2]].z));
            if(ls[x]){
                lx[x]=fsytd::min(lx[ls[x]],lx[x]);ly[x]=fsytd::min(ly[ls[x]],ly[x]);lz[x]=fsytd::min(lz[ls[x]],lz[x]);
                rx[x]=fsytd::max(rx[ls[x]],rx[x]);ry[x]=fsytd::max(ry[ls[x]],ry[x]);rz[x]=fsytd::max(rz[ls[x]],rz[x]);
            }
            if(rs[x]){
                rx[x]=fsytd::max(rx[rs[x]],rx[x]);ry[x]=fsytd::max(ry[rs[x]],ry[x]);rz[x]=fsytd::max(rz[rs[x]],rz[x]);
                lx[x]=fsytd::min(lx[rs[x]],lx[x]);ly[x]=fsytd::min(ly[rs[x]],ly[x]);lz[x]=fsytd::min(lz[rs[x]],lz[x]);
            }
        }
        //call number of mat
        int sah(int l,int r){
            int rev=r-l;if(rev<=2)return l+r>>1;bound3<T>bd,bdx;
            for(int x=l;x<=r;x++){
                const int*fc=&mesh->idx_vec[idx[x]*3];
                vec3<T,P>p0=mesh->pos[fc[0]],p1=mesh->pos[fc[1]],p2=mesh->pos[fc[2]];
                bd.add(p0),bd.add(p1),bd.add(p2);bdx.add((p0+p1+p2)/T(3));
            }
            T best=fsytd::lim<T>::max();int axis=0,place=-1;
            for(int x=0;x<3;x++){
                T mxv=(&bd.mx.x)[x],mnv=(&bd.mn.x)[x];
                if(mxv-mnv<fsytd::lim<T>::eps())continue;T e=M/(mxv-mnv);
                bound3<T>mat[M+1];int cnt[M];
                for(int y=l;y<=r;y++){
                    const int*fc=&mesh->idx_vec[idx[y]*3];
                    vec3<T,P>center=(mesh->pos[fc[0]]+mesh->pos[fc[1]]+mesh->pos[fc[2]])/T(3);
                    int b=((&center.x)[x]-mnv)*e;if(b>=M)b--;cnt[b]++;
                    mat[b].add(mesh->pos[fc[0]]);mat[b].add(mesh->pos[fc[1]]);mat[b].add(mesh->pos[fc[2]]);
                }
                int lcnt[M+1],rcnt[M+1],sl=0,sr=0;bound3<T>bdl,bdr,lmat[M],rmat[M];
                for(int y=0;y<M;y++){
                    lcnt[y]+=cnt[y];
                    bdl.add(mat[y]);
                    lmat[y].mx=bdl.mx;lmat[y].mn=bdl.mn;
                }
                for(int y=M-1;y>=0;y--){
                    rcnt[y]+=cnt[y];
                    bdr.add(mat[y]);
                    rmat[y].mx=bdr.mx;rmat[y].mn=bdr.mn;
                }
                for(int y=0;y<M;y++){
                    if(!lcnt[y]||!rcnt[y])continue;
                        T sql=weight(lmat[y]),sqr=weight(rmat[y]);
                    T sqb=weight(bd);
                    T cost=ct+ci*(lcnt[y]*sql+rcnt[y]*sqr)/sqb;
                    if(cost<best)best=cost,axis=x,place=y;
                }
            }
            T vmax=(&bdx.mx.x)[axis],vmin=(&bdx.mn.x)[axis];
            if(place==-1)return l+r>>1;
            T e=M/(vmax-vmin);
            auto it=std::partition(idx.begin()+l,idx.begin()+r,[&](int id){const int*fc=&mesh->idx_vec[id*3];
                vec3<T,P>centerx=(mesh->pos[fc[0]]+mesh->pos[fc[1]]+mesh->pos[fc[2]])/T(3);int b=((&centerx.x)[axis]-vmin)*e;
                if(b>=M)b--;return b>=place;});
            int mid=it-idx.begin();if(mid>=r||mid<=l)return l+r>>1;return mid;
        }
        T weight(bound3<T>&bd){auto dif=vec3<T,P>(bd.mx-bd.mn);auto v=max(dif,vec3<T,P>(0,0,0));return v.x*v.y+v.y*v.z+v.z*v.x;}
    };
}


