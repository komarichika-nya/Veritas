namespace PNG{
	using pii=pair<int,int>;using u16=uint16_t;using u8=uint8_t;using u32=uint32_t;u32 a[256];int ck=0;int tot=0;struct P3{int len,dis;};
    struct bit{
    	u32 buf;int cnt;vector<u8>pt;bit():buf(0),cnt(0){};
    	void wt(u32 val,int s){buf|=val<<cnt;cnt+=s;while(cnt>=8){u8 tmp=buf&0xFF;buf>>=8;cnt-=8;pt.push_back(tmp);}}
    	void fls(){if(cnt>0)pt.push_back(buf&0xFF);buf=0,cnt=0;}
        void rev(u32 val,int n){u32 ans=0;int x;for(x=0;x<n;x++)if(val&1<<(n-1-x))ans|=1<<x;wt(ans,n);}
    };
    void bd(){int x,y;for(x=0;x<256;x++){u32 c=x;for(y=0;y<8;y++)if(c&1)c=0xEDB88320^(c>>1);else c>>=1;a[x]=c;}}
    u32 crc(const u8*c,int s){bd();u32 v=0xFFFFFFFF;int x;for(x=0;x<s;x++){v=a[(v^c[x])&0xFF]^(v>>8);}return v^0xFFFFFFFF;}
    u32 rv(u32 val){return (val&0xFF000000)>>24|(val&0x00FF0000)>>8|(val&0x0000FF00)<<8|(val&0x000000FF)<<24;}
    pii huf(int val){if(val>=0&&val<=143)return mkp(val+=0x30,8);if(val>=144&&val<=255)return mkp(val+=0x190-144,9);
    if(val>=256&&val<=279)return mkp(val-=256,7);if(val>=280&&val<=287)return mkp(val=val-280+0xc0,8);return mkp(0,0);}
    int sa[N],rk[N],la[N],cnt[N],hp[N],ht[N],f[N][25];
    void SA(const vector<u8>&pt){int n=pt.size();int mx=0;
        for(auto x:pt)mx=max(mx,(int)x);int x;for(x=0;x<n;x++)++cnt[rk[x]=(int)pt[x]];
        for(x=1;x<=mx;x++)cnt[x]+=cnt[x-1];for(x=n-1;x>=0;x--)sa[--cnt[rk[x]]]=x;int p;int w;
        for(w=1;;w<<=1,mx=p){int cur=0;
            for(x=0;x<n;x++)(sa[x]+w>=n)&&(hp[cur++]=sa[x]);
            for(x=0;x<n;x++)(sa[x]>=w)&&(hp[cur++]=sa[x]-w);
            MST(cnt);for(x=0;x<n;x++)cnt[rk[x]]++;
            for(x=1;x<=mx;x++)cnt[x]+=cnt[x-1];
            for(x=n-1;x>=0;x--)sa[--cnt[rk[hp[x]]]]=hp[x];
            p=0;memcpy(la,rk,n*sizeof(int));rk[sa[0]]=0;
            auto rk2=[&](int x){return x>=n?-1:la[x];};;
            for(x=1;x<n;x++){if(la[sa[x]]==la[sa[x-1]]&&rk2(sa[x]+w)==rk2(sa[x-1]+w))rk[sa[x]]=p;else rk[sa[x]]=++p;}
            if(p==n-1)break;
        }int k;
        for(x=0,k=0;x<n;x++){if(!rk[x])continue;if(k)k--;int y=sa[rk[x]-1];while(x+k<n&&y+k<n&&pt[x+k]==pt[y+k])k++;ht[rk[x]]=k;}
    }int lg2[N];
	void pre(int n){int x;lg2[0]=lg2[1]=0;for(x=2;x<=n;x++)lg2[x]=lg2[x>>1]+1;}
    void ST(int*a,int n){pre(n);int x;for(x=0;x<n;x++)f[x][0]=a[x];int t=lg2[n];int y;for(y=1;y<=t;y++)
    for(x=0;x<=n-(1<<y);x++)f[x][y]=min(f[x][y-1],f[x+(1<<y-1)][y-1]);}
    int query(int l,int r){if(l>r)swap(l,r);int k=lg2[r-l+1];return min(f[l][k],f[r-(1<<k)+1][k]);}
    vector<pii>run(const vector<u8>&pt,int s){
        int sz=32768;set<int>wi;vector<pii>ans;int x=0;while(x<s){if(x>=sz)wi.erase(rk[x-sz]);pii bst=mkp(0,0);
        auto it=wi.lower_bound(rk[x]);auto ck=[&](int v){if(v<0||v>=s)return;int pos=sa[v];if(pos>=x)return;
        int mx=max(rk[x],v),mn=min(rk[x],v);int dis=x-pos;if(dis>32768)return;int lcp=min(query(mn+1,mx),258);
        if(lcp>bst.first)bst=mkp(lcp,dis);};if(it!=wi.end())ck(*it);if(it!=wi.begin())ck(*prev(it));int sp=1;
        if(bst.first>=3){sp=bst.first;ans.push_back(bst);}else ans.push_back(mkp(pt[x],0)),wi.insert(rk[x]);
        int y;for(y=0;y<sp;y++){if(x>=sz)wi.erase(rk[x-sz]);wi.insert(rk[x]);x++;}}return ans;
    }const int L_BASE[]={3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
    const int L_EXTRA[]={0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
    const int D_BASE[]={1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
    const int D_EXTRA[]={0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};
    vector<u8>lanch(vector<pii>&a,bit&ot){
        int n=a.size();int x;for(x=0;x<n;x++){int len=a[x].first,dis=a[x].second;
        if(!dis){auto hl=huf(len);ot.rev(hl.first,hl.second);continue;}int lc=0;
        while(lc<28&&len>=L_BASE[lc+1])lc++;auto hl=huf(257+lc);ot.rev(hl.first,hl.second);
        if(L_EXTRA[lc]>0)ot.wt(len-L_BASE[lc],L_EXTRA[lc]);int dc=0;while(dc<29&&dis>=D_BASE[dc+1])dc++;
        ot.rev(dc,5);u32 dist=dc,dr=0;int b;if(D_EXTRA[dc]>0)ot.wt(dis-D_BASE[dc],D_EXTRA[dc]);}
        auto he=huf(256);ot.rev(he.first,he.second);ot.fls();return ot.pt;
    }using vt=vector<int>;struct node{int l=-1,r=-1,feq=0,sym=-1;};
    struct tree{
        int feq[288]={0},lth[288]={0};u32 qaq[288]={0};
        void dpt(vt&a,int k){int x,n=a.size();for(x=0;x<n;x++)feq[a[x]]++;vector<node>v;
        // for(x=0;x<288;x++)if(feq[x])cerr<<feq[x]<<' '<<x<1<endl;
        priority_queue<pii,vector<pii>,greater<pii>>pq;
            for(x=0;x<288;x++)if(feq[x])v.push_back({-1,-1,feq[x],x}),pq.push(mkp(feq[x],(int)v.size()-1));
            while(pq.size()>1){auto v1=pq.top();pq.pop();auto v2=pq.top();pq.pop();
                v.push_back({v1.second,v2.second,v1.first+v2.first,-1});
            pq.push(mkp(v1.first+v2.first,v.size()-1));
            }
            dfs(v,v.size()-1,0);
            if(v.size()==1)lth[v[0].sym]=1;
            clamp(k);
        }//lth[x]>15 --> ? 
        // void clamp(int*a,int lim){}
        void clamp(int k){
            int x,mx=-1,sm=0;int tmp[32]={0};//MST(tmp);
            for(x=0;x<288;x++)if(feq[x])tmp[lth[x]]++,mx=max(mx,lth[x]);
            // cerr<<mx<<' '<<k<<endl;
            // for(x=0;x<32;x++)cerr<<tmp[x]<<' ';cerr<<endl;
            if(mx<=k)return;
            
            for(x=k+1;x<=mx;x++)sm+=tmp[x],tmp[k]+=tmp[x],tmp[x]=0;
            while(sm){
                int p=k-1;
                while(p>0&&!tmp[p])p--;
                if(p<=0)break;tmp[p]--,tmp[p+1]+=2;tmp[k]--;sm--;
            }
            vector<int>sym;
            for(x=0;x<288;x++)if(feq[x])sym.emplace_back(x);
            sort(all(sym),[&](int a,int b){if(feq[a]!=feq[b])return feq[a]<feq[b];return a<b;});
            int id=0;
            for(x=mx;x>=1;x--){
                while(tmp[x]>0)lth[sym[id++]]=x,tmp[x]--;
            }
        }
        void dfs(vector<node>&v,int p,int d){if(v.empty())return;if(v[p].l==-1&&v[p].r==-1){lth[v[p].sym]=d;return;}
        if(v[p].l!=-1)dfs(v,v[p].l,d+1);if(v[p].r!=-1)dfs(v,v[p].r,d+1);}
        void bd(){int cnt[16]={0};int x,mx=-1;for(x=0;x<288;x++)if(lth[x]>0&&lth[x]<=15)cnt[lth[x]]++;
        u32 c=0,nxt[17]={0};for(x=1;x<=15;x++)c=(c+cnt[x-1])<<1,nxt[x]=c;
        for(x=0;x<288;x++)if(lth[x])qaq[x]=nxt[lth[x]]++;
            // cerr<<endl;for(x=1;x<=15;x++)cerr<<nxt[x]<<' ';
        }
        
    };struct rle{int mk,b,val;};
    int HCLEN(int*a){int mk[]={16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};int idx=18;
    while(idx>=0&&a[mk[idx]]==0)idx--;return max(4,idx+1);}
    vector<rle>RLE(vt&mg){int p1,p2,n=mg.size();vector<rle>ans;for(p1=0;p1<n;){p2=p1;
    while(p2<n&&mg[p2]==mg[p1])p2++;int dis=p2-p1;if(mg[p1]==0){while(dis>=11){int cmp=min(dis,138);
    ans.push_back({18,7,cmp-11}),dis-=cmp;}while(dis>=3){int cmp=min(dis,10);ans.push_back({17,3,cmp-3}),dis-=cmp;}
    while(dis>0){ans.push_back({0,0,0});dis--;}}else{ans.push_back({mg[p1],0,0});dis--;while(dis>=3){
    int cmp=min(6,dis);ans.push_back({16,2,cmp-3});dis-=cmp;}while(dis>0){ans.push_back({mg[p1],0,0});dis--;}}
    p1=p2;}return ans;}
    vector<u8>huf1(vector<pii>&a,bit&ot){
        tree t1,t2;
        int n=a.size();vt v1,v2;int x;
        for(x=0;x<n;x++){if(a[x].second==0)v1.emplace_back(a[x].first);
        else{int lc=0;//cerr<<a[x].first<<endl;
            while(lc<28&&a[x].first>=L_BASE[lc+1])lc++;v1.emplace_back(257+lc);
        int dc=0;while(dc<29&&a[x].second>=D_BASE[dc+1])dc++;v2.emplace_back(dc);}}
        v1.emplace_back(256);
        t1.dpt(v1,15);t2.dpt(v2,15);
        int*l1=t1.lth,*l2=t2.lth,c1=0,c2=0;
        // for(x=0;x<288;x++)if(l2[x])cerr<<l2[x]<<' '<<x<<endl;
        int mxl=285;while(mxl>256&&t1.lth[mxl]==0)mxl--;
        int mxd=29;while(mxd>0&&t2.lth[mxd]==0)mxd--;
        int hlit=mxl+1-257,hdist=mxd;vt mg;//cerr<<mxl<<' '<<mxd<<endl;
        for(x=0;x<=mxl;x++)mg.push_back(l1[x]);//cerr<<l1[x]<<' ';cerr<<endl;
        for(x=0;x<=mxd;x++)mg.push_back(l2[x]);//cerr<<l2[x]<<' ';
        auto da=RLE(mg);
        tree t3;vt v3;
        // for(x=0;x<da.size();x++)cerr<<da[x].mk<<' '<<da[x].b<<' '<<da[x].val<<endl;
        for(x=0;x<da.size();x++)v3.push_back(da[x].mk);
        t3.dpt(v3,7);
        int*l3=t3.lth;int hclen=HCLEN(l3)-4;
        ot.wt(hlit,5);ot.wt(hdist,5);ot.wt(hclen,4);
        int mk[]={16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
        for(x=0;x<hclen+4;x++)ot.wt(t3.lth[mk[x]],3);
        t3.bd();
        // for(x=0;x<19;x++)cerr<<t3.lth[x]<<' '<<endl;
        // for(x=0;x<da.size();x++)cerr<<da[x].b<<' ';
        for(x=0;x<da.size();x++){
            // cerr<<bitset<16>(t3.qaq[da[x].mk])<<' '<<t3.lth[da[x].mk]<<endl;
            ot.rev(t3.qaq[da[x].mk],t3.lth[da[x].mk]);
            if(da[x].b>0)ot.wt(da[x].val,da[x].b);
        }
        t1.bd();t2.bd();
        for(x=0;x<n;x++){int val=a[x].first,dis=a[x].second;if(!dis)ot.rev(t1.qaq[val],t1.lth[val]);else{
        int lc=0;while(lc<28&&val>=L_BASE[lc+1])lc++;ot.rev(t1.qaq[257+lc],t1.lth[257+lc]);if(L_EXTRA[lc]>0)ot.wt(val-L_BASE[lc],L_EXTRA[lc]);
        int dc=0;while(dc<29&&dis>=D_BASE[dc+1])dc++;ot.rev(t2.qaq[dc],t2.lth[dc]);if(D_EXTRA[dc]>0)ot.wt(dis-D_BASE[dc],D_EXTRA[dc]);}}
        ot.rev(t1.qaq[256],t1.lth[256]);
        // for(x=0;x<ot.pt.size();x++)printf("%02X ",ot.pt[x]);
        ot.fls();return ot.pt;
    }
    vector<u8>press(u8*dt,int sz){
        bit tmp,ot;int x;//use compress
        ot.wt(0x78,8);ot.wt(0x01,8);ot.wt(0x05,3);
        for(x=0;x<sz;x++)tmp.wt(dt[x],8);
        SA(tmp.pt);ST(ht,tmp.pt.size());auto res=run(tmp.pt,tmp.pt.size());auto idat=huf1(res,ot);
        u32 s1=1,s2=0;for(x=0;x<sz;x++){s1=(s1+dt[x])%65521;s2=(s2+s1)%65521;}
        u32 k=s2<<16|s1;idat.push_back((k>>24)&0xff);idat.push_back((k>>16)&0xff);
        idat.push_back((k>>8)&0xff);idat.push_back(k&0xff);
        return idat;
    }
    void write(FILE*f,const char*tp,u8*ihdr,u32 s){
    	u32 len=rv(s);
		fwrite(&len,4,1,f);fwrite(tp,4,1,f);
    	if(s)fwrite(ihdr,1,s,f);vector<u8>tmp;int l=strlen(tp);int x;
    	for(x=0;x<l;x++)tmp.push_back(tp[x]);
    	tmp.insert(tmp.end(),ihdr,ihdr+s);
    	u32 ans=rv(crc(tmp.data(),tmp.size()));
    	fwrite(&ans,4,1,f);
    }
    void sv(const char*f,int w,int h,const float*rgb){
    	FILE*ptr=fopen(f,"wb");assert(ptr!=nullptr);
    	u8 mk[]={0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};fwrite(mk,1,8,ptr);
    	u8 col=2;u8 ihdr[13];u32 w1=rv(w);u32 h1=rv(h);*(u32*)ihdr=w1;*(u32*)(ihdr+4)=h1;
    	ihdr[8]=8;ihdr[9]=col;ihdr[10]=0;ihdr[11]=0;ihdr[12]=0;vector<u8>rgb8;
    	write(ptr,"IHDR",ihdr,13);
		int x,y;for(y=0;y<h;y++){rgb8.push_back(0);for(x=0;x<w*3;x++)rgb8.push_back((u8)(rgb[y*w*3+x]*255.0f));}
		auto idat=press(rgb8.data(),rgb8.size());
        write(ptr,"IDAT",idat.data(),idat.size());write(ptr,"IEND",nullptr,0);
    }
    //using ld=double;u32 rd(const u8*p){return p[0]<<24|p[1]<<16|p[2]<<8|p[3];}
    //8byte is "IDAT" -> 0x78 8bit,0x01 8bit,->filer,1 byte :data[r,g,b,r,g,b...]
    // wt(val,bit)->vector<u8>output->hufuman decode
    // struct byte{const u8*p;u32 buf;int cnt;byte(const u8*p1):p(p1),buf(0),cnt(0){};
    // u32 wt(int n){while(cnt<n){buf|=(u32)(*p++)<<cnt;cnt+=8;}u32 val=buf&((1u<<n)-1);buf>>=n;cnt-=n;return val;}
    // void rset(){buf=cnt=0;}};
    // void read(const u8*p,vector<u32>&idat,byte&b){int x=0;while(*p++!=0x54)idat.push_back(hufuman(b.wt(8)));}
    // void decode(const char*f){
    //     int fd=open(f,RD_ONLY);if(f<0)return;
    //     struct stat st;fstat(fd,&st);
    //     size_t sz=st.st_size;u8*data=(u8*)mmap(NULL,sz,PROT_READ,MAP_PRIVATE,fd,0);close(fd);
    //     madvise(data,sz,MADV_SEQUENTIAL);if(data==MAP_FAILED)return;
    //     u8*p=data;int x=0;u8 mk[]={0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};
    //     if(!memcmp(p,mk,8)){munmap(data,sz);return;}p+=8;
    //     u8*p1=data;u32 w1,h1;u8 dep,tp,ps,fi,itr;u32 ga=0;vector<u32>idat;
    //     while(p1<data+sz){
    //         u32 lth=*(u32*)p1;p1+=4;u32 tp1=*(u32*)p1;p1+=4;
    //         if(tp1==0x52444849){w1=rd(p1);h1=rd(p1+4);dep=*(p1+5);tp=*(p1+6);ps=*(p1+7);fi=*(p1+8);itr=*(p1+9);}
    //         if(tp1==0x67414d41)ga=(u32*)p1/100000.0;
    //         if(tp1==0x49484452){}
    //         if(tp1==0x54414449){byte b(p1+2);}p1+=lth;
    //     }
    // }
}
