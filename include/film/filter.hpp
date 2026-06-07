#pragma once
#include"conf/conf.hpp"
#include"core/vec.hpp"
#include"film/filterSample.hpp"
template<typename tp,typename T>
class Filter{
public:
HD vec2<T,P>radius();
HD T evaluate(const vec2<T,P>&p);
HD T integral();
HD filterSample<T>sample(const vec2<T,P>&u)const;
};
template<typename T>
class Box:public Filter<Box<T>,T>{
public:
HD Box();
HD Box(const vec2<T,P>&r);
HD vec2<T,P>radius()const;
HD T integral()const;
HD T evaluate(const vec2<T,P>&p)const;
HD filterSample<T>sample(const vec2<T,P>&v)const;
private:vec2<T,P>r;};
#include"../../src/film/filter.cpp"
