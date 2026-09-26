#pragma once
#include<cstdio>
#include<cstring>
#include<cassert>
#include<vector>
#include<numeric>
#include"core/vec.hpp"
#include"util/scene.hpp"
namespace veritas{
    namespace fsytd{
        template<typename T>Scene<T>reader(const char*name){
            FILE*ptr=fopen(name,"rb");assert(ptr!=nullptr&&"failed to open file");
            char head[5];head[4]='\0';fread(&head,sizeof(char),4,ptr);assert(strcmp(head,"\nVES")==0&&"invaild file");
            int swh[6];fread(&swh,sizeof(int),6,ptr);
            //printf("%s %d %d %d %d %d %d\n",head,swh[0],swh[1],swh[2],swh[3],swh[4],swh[5]);
            int spp=swh[0],w=swh[1],h=swh[2],posn=swh[3],facen=swh[4],normaln=swh[5];
            vec3<T,P>pos[posn];fread(&pos,sizeof(vec3<T,P>),posn,ptr);
            std::vector<vec3<T,P>>ps(pos,pos+posn);            
            //std::iota(tri_idx.begin(),tri_idx.end(),0);
            int faces[facen];fread(&faces,sizeof(int),facen,ptr);
            std::vector<int>idx(faces,faces+facen);
            vec3<T,V>norm[normaln];fread(&norm,sizeof(vec3<T,V>),normaln,ptr);
            std::vector<vec3<T,V>>normal(norm,norm+normaln);
            //printf("%d %d %d %d\n",ps.size(),tri_idx.size(),idx.size(),normal.size());
            //int x;for(x=0;x<ps.size();x++)printf("%f %f %f\n",ps[x].x,ps[x].y,ps[x].z);
            //for(x=0;x<idx.size();x++)printf("%d\n",idx[x]);
            //for(x=0;x<normal.size();x++)printf("%f %f %f\n",normal[x].x,normal[x].y,normal[x].z);
            int32_t caminfo[2];fread(&caminfo,sizeof(int32_t),2,ptr);
            //printf("%d %d\n",caminfo[0],caminfo[1]);
            T ca[4];//fread(&ca,sizeof(float),4,ptr);printf("%f %f %f %f\n",ca[0],ca[1],ca[2],ca[3]);
            //0x43b     0x139
            //switcher
            fclose(ptr);
            return Scene<T>(w,h,spp,ps,idx,normal);
        }
    }//namespace fsytd
}//namespace veritas
