namespace veritas{
namespace fsytd{
    template<typename T>class allocator{
        using value_type=T;
        inline static char*start=nullptr;
        inline static char*end=nullptr;
        inline static size_t sz=0;
        union node{node*nxt;alignas(T)char buf[sizeof(T)];};
        enum{MIN_SIZE=8,MAX_SIZE=128,NEXT_FREE=16,MEMBER_SIZE=20};
        inline static node*list[NEXT_FREE]={};
        inline static std::vector<char*>chunks{};
        inline static std::mutex pool{};
        //n byte
        static size_t up(size_t n){return(n+MIN_SIZE-1)&~(MIN_SIZE-1);}
        static size_t idx(size_t n){return(n+MIN_SIZE-1)/MIN_SIZE-1;}
        static node*fill(size_t n){
            size_t t=MEMBER_SIZE;
            char*ck=chunk_alloc(n,t);if(t==1)return reinterpret_cast<node*>(ck);
            node*head=reinterpret_cast<node*>(ck);
            size_t id=idx(n);list[id]=reinterpret_cast<node*>(ck+n);
            node*cur=list[id];size_t x=1;
            //for x in range(all_menber)
            for(;x<t-1;x++)cur->nxt=reinterpret_cast<node*>(ck+(x+1)*n),cur=cur->nxt;
            cur->nxt=nullptr;return head;
        }
        static char*chunk_alloc(size_t n,size_t&obj){
            //total byte.
            size_t tot=n*obj;size_t l=start&&end?end-start:0;
            if(l>=tot){char*tmp=start;start+=tot;return tmp;}
            if(l>=n){obj=l/n;tot=n*obj;char*tmp=start;start+=tot;return tmp;}
            size_t apply=2*tot+up(sz>>4);
            if(l>0){size_t id=idx(l);reinterpret_cast<node*>(start)->nxt=list[id];list[id]=reinterpret_cast<node*>(start);}
            start=static_cast<char*>(::operator new(apply));end=start+apply;chunks.push_back(start);
            sz+=apply;return chunk_alloc(n,obj);
        }
    public:
        constexpr allocator()=default;
        constexpr allocator&operator=(const allocator&)=default;
        constexpr ~allocator()=default;
        template<typename U>struct rebind{using other=allocator<U>;};
        template<typename U>constexpr allocator(const allocator<U>&){};
        //n number
        T*allocate(size_t np){
            if(np==0)return nullptr;
            size_t n=np*sizeof(T);
            if(n>MAX_SIZE)return static_cast<T*>(::operator new(n));
            std::lock_guard<std::mutex>g(pool);
            size_t id=idx(up(n));node*tmp=list[id];if(tmp==nullptr)return reinterpret_cast<T*>(fill(up(n))->buf);
            list[id]=tmp->nxt;return reinterpret_cast<T*>(tmp->buf);
        }
        //n number
        void deallocate(T*p,size_t np){
            size_t n=np*sizeof(T);
            if(n>MAX_SIZE){::operator delete(p);return;}
            std::lock_guard<std::mutex>g(pool);
            size_t id=idx(up(n));node*nd=reinterpret_cast<node*>(p);
            nd->nxt=list[id];list[id]=nd;
        }
        static void release(){
            std::lock_guard<std::mutex>g(pool);
            for(char*c:chunks)::operator delete(c);
            chunks.clear();chunks.shrink_to_fit();
            start=end=nullptr;sz=0;for(auto&h:list)h=nullptr;
        }
        template<typename U,typename...tp>
        void construct(U*p,tp&...args){::new(static_cast<void*>(p))U(fsytd::forward_<tp>(args)...);}
        template<typename U>
        void destory(U*p){p->~U();}
    };
}//namespace fsytd
}//namespace veritas
