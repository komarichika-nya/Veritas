namespace fsytd{
    template<typename T>class allocator{
        using value_type=T;
        char*start=nullptr;char*end=nullptr;size_t sz=0;
        constexpr allocator()=default;
        constexpr allocator&operator=(const allocator&)=default;
        constexpr ~allocator()=default;
        union node{node*nxt;char buf[1];};
        enum{MIN_SIZE=8,MAX_SIZE=128,NEXT_FREE=16,MEMBER_SIZE=20};
        node*list[NEXT_FREE]={};
        //n byte
        size_t up(size_t n){return(n+MIN_SIZE-1)&~(MIN_SIZE-1);}
        size_t idx(size_t n){return(n+MIN_SIZE-1)/MIN_SIZE-1;}
        void*fill(size_t n){
            size_t t=MEMBER_SIZE;
            char*ck=chunk_alloc(n,t);if(t==1)return ck;
            node*head=reinterpret_cast<node*>(ck);
            size_t id=idx(n);list[id]=reinterpret_cast<node*>(ck+n);
            node*cur=list[id];size_t x=1;
            //for x in range(all_menber)
            for(;x<t-1;x++)cur->nxt=reinterpret_cast<node*>(ck+(x+1)*n),cur=cur->nxt;
            cur->nxt=nullptr;return head;
        }
        char*chunk_alloc(size_t n,size_t&obj){
            //total byte.
            size_t tot=n*obj;size_t l=start&&end?end-start:0;
            if(l>=tot){char*tmp=start;start+=tot;return tmp;}
            if(l>=n){obj=l/n;tot=n*obj;char*tmp=start;start+=tot;return tmp;}
            size_t apply=2*tot+up(sz>>4);
            if(l>0){size_t id=idx(l);reinterpret_cast<node*>(start)->nxt=list[id];list[id]=reinterpret_cast<node*>(start);}
            start=static_cast<char*>(::operator new(apply));
            sz+=apply;return chunk_alloc(n,obj);
        }
    public:
        //n number
        T*allocate(size_t np){
            size_t n=np*sizeof(T);
            if(n>MAX_SIZE)return static_cast<T*>(::operator new(n*sizeof(T)));
            size_t id=idx(up(n));node*tmp=list[id];if(tmp==nullptr)return fill(up(n));
            list[id]=tmp->nxt;return tmp;
        }
        //n number
        void deallocate(T*p,size_t np){
            size_t n=np*sizeof(T);
            if(n>MAX_SIZE){::operator delete(p);return;}
            size_t id=idx(up(n));node*nd=static_cast<node*>(p);
            nd->nxt=list[id];list[id]=nd;
        }
    };
}//namespace fsytd
