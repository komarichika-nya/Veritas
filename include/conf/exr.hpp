namespace fsytd{
    namespace EXR{
        void sv(const char*f,int w,int h,const float*rgb){
            FILE*ptr=fopen(f,"wb");assert(ptr!=nullptr);
            u8 num[]={0x76,0x2f,0x31,0x01};fwrite(num,1,4,ptr);
        u8 ver[]={0x02,0x00,0x00,0x00};fwrite(ver,1,4,ptr);
        auto write=[&](const char*a,const char*b,int s,const void*d){
            fprintf(ptr,"%s%c",a,0);fprintf(ptr,"%s%c",b,0);fwrite(&s,1,4,ptr);
        fwrite(d,1,s,ptr);};
        u8 l[]={'B',0, 2,0,0,0, 0,0,0,0, 1,0,0,0, 1,0,0,0,
                'G',0, 2,0,0,0, 0,0,0,0, 1,0,0,0, 1,0,0,0,
                'R',0, 2,0,0,0, 0,0,0,0, 1,0,0,0, 1,0,0,0, 0};
        write("channels","chlist",sizeof(l),l);
        int ps=0;write("compression","compression",1,&ps);
        int box[]={0,0,w-1,h-1};write("dataWindow","box2i",16,box);
        write("displayWindow","box2i",16,box);
        int cnt=0;write("lineOrder","lineOrder",1,&cnt);
        float ace=1.0f;write("pixelAspectRatio","float",4,&ace);float v2f[]={0,0};
        write("screenWindowCenter","v2f",8,v2f);
        float sww=1.0f;write("screenWindowWidth","float",4,&sww);
        fputc(0,ptr);long pos=ftell(ptr);
        vector<ll>tmp(h+1);int x;
        for(x=0;x<h;x++){ll v=0;fwrite(&v,1,8,ptr);}
        vector<float>r(w+1),g(w+1),b(w+1);int y;
        for(y=0;y<h;y++){
            tmp[y]=ftell(ptr);fwrite(&y,1,4,ptr);
            int rs=w*3*4;fwrite(&rs,1,4,ptr);
            for(x=0;x<w;x++)r[x]=rgb[(y*w+x)*3],g[x]=rgb[(y*w+x)*3+1],b[x]=rgb[(y*w+x)*3+2];
            fwrite(b.data(),4,w,ptr);fwrite(g.data(),4,w,ptr);fwrite(r.data(),4,w,ptr);
        }
            fseek(ptr,pos,SEEK_SET);fwrite(tmp.data(),8,h,ptr);fclose(ptr);
        }
    }
}//namespace fsystd
