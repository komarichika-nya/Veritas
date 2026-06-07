#include<iostream>
#include<cassert>
#include<algorithm>
using std::pow;
using std::clamp;
using std::ceil;
using std::floor;
template<typename T>
template<typename F>
void Tile<T>::addSample(const F&f,const vec2<T,P>&p,const Spectrum<T>&L,T w){
vec2<T,P>r=f.radius();
int x0=max((int)ceil(p.x-r.x+T(0.5)),sampleBound.mn.x);
int x1=min((int)floor(p.x+r.x+T(0.5)),sampleBound.mx.x);
int y0=max((int)ceil(p.y-r.y+T(0.5)),sampleBound.mn.y);
int y1=min((int)floor(p.y+r.y+T(0.5)),sampleBound.mx.y);
int x,y;int rw=sampleBound.mx.x-sampleBound.mn.x+1;
for(y=y0;y<=y1;y++){
    for(x=x0;x<=x1;x++){
        vec2<T,P>p2f(T(x)-p.x+T(0.5),T(y)-p.y+T(0.5));
        T we=f.evaluate(p2f);
        Spectrum<T>col;
        col=L*w*we;
        //assert(we>0);
        int idx=(y-sampleBound.mn.y)*rw+(x-sampleBound.mn.x);
        //assert(idx>=0&&idx<=(sampleBound.mx.y-sampleBound.mn.y+1));error
        //printf("%f %f %f\n",col.c[0],col.c[1],col.c[2]);
        pixel[idx].rgb[0]+=col.c[0];
        pixel[idx].rgb[1]+=col.c[1];
        pixel[idx].rgb[2]+=col.c[2];
        pixel[idx].sum+=we;
    }
}}
template<typename F,typename T>
Film<F,T>::Film()=default;
template<typename F,typename T>
Film<F,T>::Film(const vec2<int,P>&full,const bound2<int>&crop):full(full),crop(crop),pixel(std::make_unique<Pixel<T>[]>(full.x*full.y)){}
template<typename F,typename T>
Tile<T>Film<F,T>::getTile(const bound2<int>&b){
    Tile<T>t(b);auto r=filter.radius();
    vec2<float,P>mnf((float)b.mn.x-r.x,(float)b.mn.y-r.y);
    vec2<float,P>mxf((float)b.mx.x+r.x,(float)b.mx.y+r.y);
    t.sampleBound=bound2<int>{vec2<int,P>{(int)std::floor(mnf.x),(int)std::floor(mnf.y)},vec2<int,P>{(int)std::ceil(mxf.x),(int)std::ceil(mxf.y)}};
    //printf("mx_x:%d mx_y:%d mn_x:%d mn_y:%d area:%d\n",t.sampleBound.mx.x,t.sampleBound.mx.y,t.sampleBound.mn.x,t.sampleBound.mn.y,t.sampleBound.area());
    t.pixel=std::make_unique<Pixel<T>[]>(t.sampleBound.area());
    return t;}
template<typename F,typename T>
void Film<F,T>::mergeTile(Tile<T>&t){
    std::lock_guard<std::mutex>lock(mergeMutex);
    int x,y,rw=t.sampleBound.mx.x-t.sampleBound.mn.x+1,rh=t.sampleBound.mx.y-t.sampleBound.mn.y+1;
    for(y=t.sampleBound.mn.y;y<=t.sampleBound.mx.y;y++){
        for(x=t.sampleBound.mn.x;x<=t.sampleBound.mx.x;x++){
            int gb=y*full.x+x,lcp=(y-t.sampleBound.mn.y)*rw+(x-t.sampleBound.mn.x);
            //assert(lcp>=0&&lcp<=t.sampleBound.mx.y-t.sampleBound.mn.y+1);
            pixel[gb].rgb[0]+=t.pixel[lcp].rgb[0];
            pixel[gb].rgb[1]+=t.pixel[lcp].rgb[1];
            pixel[gb].rgb[2]+=t.pixel[lcp].rgb[2];
            pixel[gb].sum+=t.pixel[lcp].sum;
        }
    }
}
template<typename F,typename T>
void Film<F,T>::addSplat(const vec2<T,P>&p,const Spectrum<T>&L,T w){
    int x=static_cast<int>(p.x),y=static_cast<int>(p.y);
    int idx=y*full.x+x;
    pixel[idx].splatXYZ[0]+=L.c[0]*w;
    pixel[idx].splatXYZ[1]+=L.c[1]*w;
    pixel[idx].splatXYZ[2]+=L.c[2]*w;
    pixel[idx].splatSum+=w;
}
//sample, for test
template<typename F,typename T>
void Film<F,T>::writeImage(const char*name){
//std::cout<<full.y<<' '<<full.x<<'\n';
int x,y;FILE*f=fopen(name,"w");assert(f!=nullptr);
//const char*s="P3";
fprintf(f,"P3\n%d %d\n255\n",full.x,full.y);
for(y=0;y<full.y;y++)
    for(x=0;x<full.x;x++){
        int idx=y*full.x+x;
        float s=pixel[idx].sum;
        if(s<=0){fprintf(f,"0 0 0\n");continue;}
        Spectrum<T>col;
        col.c[0]=pixel[idx].rgb[0]/pixel[idx].sum;//+pixel[idx].splatXYZ[0]/pixel[idx].splatSum;
        col.c[1]=pixel[idx].rgb[1]/pixel[idx].sum;//+pixel[idx].splatXYZ[1]/pixel[idx].splatSum;
        col.c[2]=pixel[idx].rgb[2]/pixel[idx].sum;//+pixel[idx].splatXYZ[2]/pixel[idx].splatSum;
        auto gamma=[](T v){return pow(clamp(v,T(0),T(1)),T(1)/T(2));};
        int r=int(gamma(col.c[0])*T(255.99)),g=int(gamma(col.c[1])*T(255.99)),b=int(gamma(col.c[2])*T(255.99));
        fprintf(f,"%d %d %d\n",int(r),int(g),int(b));
    }
}

