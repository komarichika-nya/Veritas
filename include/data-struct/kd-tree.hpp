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
    template<typename T>class Kd_tree{
    public:
        Kd_tree(std::vector<>)
        //return the root index
        int build(int l,int r){
            if(l>r)return 0
            int mid=sah(l,r);
            child[0][mid]=build(l,mid);
            child[1][mid]=build(mid+1,r);
            update(mid);
            return mid;
        }
    private:
        std::vector<T>child[2];
        std::vector<int>index;
        void update(int x){
            
        }
    };
}


