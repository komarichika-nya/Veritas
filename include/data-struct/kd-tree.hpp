//deeply love for data-struct
#pragma once
#include<vector>
#include<cstddef>
#include"conf/optional.hpp"
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
        const Mesh<T>*tri;
        int root=-1;
        Kd_tree()=default;
        Kd_tree(const Mesh<T>&m,const SplitMode&mode,T ct,T ci){
            this->tri=&m;
            this->ci=ci;this->ct=ct;
            idx.resize(tri->getMesh()->num_vec/3);
            std::iota(idx.begin(),idx.end(),0);
            init();
            switch(mode){
                case SplitMode::sah:root=this->build<SplitMode::sah>(0,static_cast<int>(idx.size())-1);
                case SplitMode::middle:root=this->build<SplitMode::middle>(0,static_cast<int>(idx.size())-1);
            }
        }
    private:
        T ci,ct;static constexpr int M=12;//M is called number of mat
        int*ls=nullptr,*rs=nullptr;T*lx=nullptr,*rx=nullptr,*ly=nullptr,*ry=nullptr,*lz=nullptr,*rz=nullptr;
        
        //return the root index
        template<SplitMode mode>
        int build(int l,int r){
            if(l>r)return 0;
            int mid=0;
            if constexpr(mode==SplitMode::sah)mid=sah(l,r);
            if constexpr(mode==SplitMode::middle)mid=l+r>>1;
            ls[mid]=this->build<mode>(l,mid-1);
            rs[mid]=this->build<mode>(mid+1,r);
            update(mid);
            return mid;
        }
    private:
        void init(){
            fsytd::allocator<int>alloc_int;
            fsytd::allocator<T>alloc_T;
            auto view=tri->getMesh();
            ls=alloc_int.allocate(view->num_idx);
            rs=alloc_int.allocate(view->num_idx);
            lx=alloc_T.allocate(view->num_vec);
            rx=alloc_T.allocate(view->num_vec);
            ly=alloc_T.allocate(view->num_vec);
            ry=alloc_T.allocate(view->num_vec);
            lz=alloc_T.allocate(view->num_vec);
            rz=alloc_T.allocate(view->num_vec);
        }
        void update(int x){
            int q=idx[x];auto view=tri->getMesh();const int*id=&view->idx_vec[q*3];
            lx[x]=fsytd::min(view->pos[id[0]].x,fsytd::min(view->pos[id[1]].x,view->pos[id[2]].x));
            rx[x]=fsytd::max(view->pos[id[0]].x,fsytd::max(view->pos[id[1]].x,view->pos[id[2]].x));
            ly[x]=fsytd::min(view->pos[id[0]].y,fsytd::min(view->pos[id[1]].y,view->pos[id[2]].y));
            ry[x]=fsytd::max(view->pos[id[0]].y,fsytd::max(view->pos[id[1]].y,view->pos[id[2]].y));
            lz[x]=fsytd::min(view->pos[id[0]].z,fsytd::min(view->pos[id[1]].z,view->pos[id[2]].z));
            rz[x]=fsytd::max(view->pos[id[0]].z,fsytd::max(view->pos[id[1]].z,view->pos[id[2]].z));
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
            auto view=tri->getMesh();
            int rev=r-l;if(rev<=2)return l+r>>1;bound3<T>bd,bdx;
            for(int x=l;x<=r;x++){
                //printf("%d %d %d %d\n",x,fc[0],fc[1],fc[2]);
                const int*fc=&view->idx_vec[idx[x]*3];
                //printf("x:%d fc[0]:%d fc[1]:%d fc[2]:%d %d %d\n",x,fc[0],fc[1],fc[2],mesh->num_idx,mesh->num_vec);
                vec3<T,P>p0=view->pos[fc[0]],p1=view->pos[fc[1]],p2=view->pos[fc[2]];
                bd.add(p0),bd.add(p1),bd.add(p2);bdx.add((p0+p1+p2)/T(3));
            }
            T best=fsytd::lim<T>::max();int axis=0,place=-1;
            for(int x=0;x<3;x++){
                T mxv=(&bd.mx.x)[x],mnv=(&bd.mn.x)[x];
                if(mxv-mnv<fsytd::lim<T>::eps())continue;T e=M/(mxv-mnv);
                bound3<T>mat[M+1];int cnt[M]={};
                for(int y=l;y<=r;y++){
                    const int*fc=&view->idx_vec[idx[y]*3];
                    vec3<T,P>center=(view->pos[fc[0]]+view->pos[fc[1]]+view->pos[fc[2]])/T(3);
                    int b=((&center.x)[x]-mnv)*e;if(b>=M)b--;cnt[b]++;
                    mat[b].add(view->pos[fc[0]]);mat[b].add(view->pos[fc[1]]);mat[b].add(view->pos[fc[2]]);
                }
                int lcnt[M+1]={},rcnt[M+1]={},sl=0,sr=0;bound3<T>bdl,bdr,lmat[M],rmat[M];
                for(int y=0;y<M;y++){
                    lcnt[y]=cnt[y]+(y>0?cnt[y-1]:0);
                    bdl.add(mat[y]);
                    lmat[y].mx=bdl.mx;lmat[y].mn=bdl.mn;
                }
                for(int y=M-1;y>=0;y--){
                    rcnt[y]=cnt[y]+(y+1<M?rcnt[y+1]:0);
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
            auto it=std::partition(idx.begin()+l,idx.begin()+r+1,[&](int id){const int*fc=&view->idx_vec[id*3];
                vec3<T,P>centerx=(view->pos[fc[0]]+view->pos[fc[1]]+view->pos[fc[2]])/T(3);int b=((&centerx.x)[axis]-vmin)*e;
                if(b>=M)b--;return b>=place;});
            int mid=it-idx.begin();if(mid>r||mid<=l)return l+r>>1;return mid;
        }
        T weight(bound3<T>&bd){auto dif=vec3<T,P>(bd.mx-bd.mn);auto v=max(dif,vec3<T,P>(0,0,0));return v.x*v.y+v.y*v.z+v.z*v.x;}
        bool check(int x,ray<T>*r){
            T tmn=-fsytd::lim<T>::max(),tmx=fsytd::lim<T>::max();T tx1=(lx[x]-r->o.x)*r->d.x;
            T tx2=(rx[x]-r->o.x)*r->d.x;tmn=fsytd::max(tmn,fsytd::min(tx1,tx2));tmx=fsytd::min(tmx,fsytd::max(tx1,tx2));
            T ty1=(ly[x]-r->o.y)*r->d.y;T ty2=(ry[x]-r->o.y)*r->d.y;tmn=fsytd::max(tmn,fsytd::min(ty1,ty2));
            tmx=fsytd::min(tmx,fsytd::max(ty1,ty2));
            T tz1=(lz[x]-r->o.z)*r->d.z;T tz2=(rz[x]-r->o.z)*r->d.z;tmn=fsytd::max(tmn,fsytd::min(tz1,tz2));
            tmx=fsytd::min(tmx,fsytd::max(tz1,tz2));return tmx>=fsytd::max(T(0),tmn);
        }
    public:
        ~Kd_tree(){
            auto view=tri->getMesh();
            fsytd::allocator<int>alloc_int;fsytd::allocator<T>alloc_T;
            if(ls)alloc_int.deallocate(ls,view->num_idx);
            if(rs)alloc_int.deallocate(rs,view->num_idx);
            if(lx)alloc_T.deallocate(lx,view->num_vec);
            if(rx)alloc_T.deallocate(rx,view->num_vec);
            if(ly)alloc_T.deallocate(ly,view->num_vec);
            if(ry)alloc_T.deallocate(ry,view->num_vec);
            if(lz)alloc_T.deallocate(lz,view->num_vec);
            if(rz)alloc_T.deallocate(rz,view->num_vec);
            fsytd::allocator<int>::release();
            fsytd::allocator<T>::release();
        }
        void ask(int node,ray<T>*r,surface<T>&sur){
            if(node==-1||!check(node,r))return;
            auto s=this->tri->intersect(node,r);
            if(s.has()&&s.val().t<sur.t)sur=s.val();
            ask(ls[node],r,sur);ask(rs[node],r,sur);
        } 
    };
}


