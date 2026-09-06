//deeply love for data-struct
#pragma once
#include<vector>
#include<cstddef>
#include"conf/conf.hpp"
#include"conf/span.hpp"
#include"util/shape.hpp"
#include"core/bound.hpp"
#include"core/vec.hpp"
namespace veritas{
    enum SplitMode{sah,middle};
    template<typename T>class Kd_tree{
    public:
        int*idx=nullptr;
        Kd_tree()=default;
        Kd_tree(const Shape<Triangle,T>&mesh){
            this->num_idx=mesh->num_idx;
            this->num_vec=mesh->num_vec;
            this->idx_vec=&mesh->idx_vec;
            this->pos=&mesh->pos;
            init();
        }
        //return the root index
        int build(int l,int r,SplitMode&mode){
            if(l>r)return 0
            int mid=0;
            if constexpr(mode==SplitMode::sah)mid=sah(l,r);
            if constexpr(mode==SplitMode::mid)mid=l+r>>1;
            ls[mid]=build(l,mid);
            rs[mid]=build(mid+1,r);
            update(mid);
            return mid;
        }
    private:
        int num_idx=-1,num_vec=-1;
        const int*idx_vec=nullptr;
        const vec3<T,P>*pos=nullptr;
        int*ls=nullptr,*rs=nullptr;T*lx=nullptr,*rx=nullptr,*ly=nullptr,*ry=nullptr,*lz=nullptr,*rz=nullptr;
        void init(){
            allocator<int>alloc_int;
            allocator<T>alloc_T;
            ls=alloc_int.allocate(num_idx);
            rs=alloc_int.allocate(num_idx);
            lx=alloc_T.allocate(num_vec);
            rx=alloc_T.allocate(num_vec);
            ly=alloc_T.allocate(num_vec);
            ry=alloc_T.allocate(num_vec);
            lz=alloc_T.allocate(num_vec);
            rz=alloc_T.allocate(num_vec);
        }
        void update(int x){
            int q=idx[x];int*id=idx_vec[q];
            lx[x]=fsytd::min(pos[id[0]].x,fsytd::min(pos[id[1]].x,pos[id[2]].x));
            rx[x]=fsytd::max(pos[id[0]].x,fsytd::max(pos[id[1]].x,pos[id[2]].x));
            ly[x]=fsytd::min(pos[id[0]].y,fsytd::min(pos[id[1]].y,pos[id[2]].y));
            ry[x]=fsytd::max(pos[id[0]].y,fsytd::max(pos[id[1]].y,pos[id[2]].y));
            lz[x]=fsytd::min(pos[id[0]].z,fsytd::min(pos[id[1]].z,pos[id[2]].z));
            rz[x]=fsytd::max(pos[id[0]].z,fsytd::max(pos[id[1]].z,pos[id[2]].z));
            if(ls[x]){
                lx[x]=fsytd::min(lx[ls[x]],lx[x]);ly[x]=fsytd::min(ly[ls[x]],ly[x]);lz[x]=fsytd::min(lz[ls[x]],lz[x]);
                rx[x]=fsytd::max(rx[ls[x]],rx[x]);ry[x]=fsytd::max(ry[ls[x]],ry[x]);rz[x]=fsytd::max(rz[ls[x]],rz[x]);
            }
            if(rs[x]){
                rx[x]=fsytd::max(rx[rs[x]],rx[x]);ry[x]=fsytd::max(ry[rs[x]],ry[x]);rz[x]=fsytd::max(rz[rs[x]],rz[x]);
                lx[x]=fsytd::min(lx[rs[x]],lx[x]);ly[x]=fsytd::min(ly[rs[x]],ly[x]);lz[x]=fsytd::min(lz[rs[x]],lz[x]);
            }
        }
        int num_mat=12;
        int sah(int l,int r){
            int rev=r-l;if(len<=2)return l+r>>1;bound3<T>bd,bdx;
            for(int x=l;x<=r;x++){
                int*fc=idx_vec[idx[x]];
                vec3<T,P>p0=pos[fc[0]],p1=pos[fc[1]],p2=pos[fc[2]];
                bd.add(p0),bd.add(p1),bd.add(p2);bdx.add((p0+p1+p2)/T(3));
            }
            T best=fsytd::lim<T>::max(),axis=0,place=-1;
            for(int x=0;x<3;x++){
                T mxv=(&bd.mx.x)[x],mnv=(&bd.mn.x)[x];
                if(mxv-mnv<lim<T>::eps())continue;T e=M/(mxv-mnv);
                bound3<T>mat[M+1];int cnt[M];
                for(int y=l;y<=r;y++){
                    int*fc=idx_vec[idx[y]]
                    vec3<T,P>center=(pos[fc[0]]+pos[fc[1]]+pos[fc[2]])/T(3);
                    int b=(&(center.mx.x)[x]-mnv)*e;if(b>=M)b--;cnt[b]++;
                    mat.add(pos[fc[0]]);mat.add(pos[fc[1]]);mat,add(pos[fc[2]]);
                }
                int lcnt[M+1],rcnt[M+1],sl=0,sr=0;bound3<T>bdl,bdr,lmat[M],rmat[M];
                for(int y=0;y<M;y++){
                    lcnt[y]+=cnt[y];
                    bdl.add(mat[y]);
                    lmat[y].mx=bdl.mx;lmat.mn=bdl.mn;
                }
                for(int y=M-1;y>=0;y--){
                    rcnt[y]+=cnt[y];
                    bdr.add(mat[y]);
                    rmat[y].mx=bdr.mx;rmat.mn=bdr.mn;
                }
                for(int y=0;y<M;y++){
                    if(!lcnt[y]||!rcnt[y])continue;
                    T sql=sq(lmat),sqr=sq(rmat);
                    T sqb=sq(bd);
                    T cost=ct+ci*(lcnt[y]*sql+rcnt[y]*sqr)/sqb;
                    if(cost<best)best=cost,axis=x,place=y;
                }
            }
            T vmax=&(bdx.mx.x)[zb],vmin=&(bdx.mn.x)[zb];
            if(place==-1)return l+r>>1;
            T e=M/(vmax-vmin);
            auto it=std::partition(idx+l,idx+r,[](int id){int*fc=idx_vec[id];
                vec3<T,P>centerx=(pos[fc[0]]+pos[fc[1]]+pos[fc[2]])/T(3);int b=((&centerx.x)[zb]-vmin)*e;
                if(b>=M)b--;return b>=place;});
            int mid=it-idx;if(mid>=r||mid<=l)return l+r>>1;return mid;
        }
    };
}


