#pragma once
#include<mutex>
#include<memory>
#include<atomic>
#include"conf/conf.hpp"
#include"core/bound.hpp"
#include"core/vec.hpp"
#include"film/pixel.hpp"
#include"film/filter.hpp"
#include"color/rgb.hpp"
//#include"film/image/exr.hpp"
template<typename T>struct Tile{
//Pixel<T>*pixel;
std::unique_ptr<Pixel<T>[]>pixel;
bound2<int>pixelBound;bound2<int>sampleBound;
Tile()=delete;
Tile(const Tile&)=delete;
Tile&operator=(const Tile&)=delete;
Tile(Tile&&)noexcept=default;
Tile&operator=(Tile&&)noexcept=default;
Tile(const bound2<int>&b):pixelBound(b){}
template<typename F>
void addSample(const F&f,const vec2<T,P>&p,const Spectrum<T>&l,T w);
~Tile()=default;};
template<typename F,typename T>struct Film{
//Pixel<T>pixel;
std::unique_ptr<Pixel<T>[]>pixel;
vec2<int,P>full;bound2<int>crop;F filter;std::mutex mergeMutex;
Film();
Film(const vec2<int,P>&full,const bound2<int>&crop);
Tile<T>getTile(const bound2<int>&b);
void mergeTile(Tile<T>&t);
void addSplat(const vec2<T,P>&p,const Spectrum<T>&L,T w);
void writeImage(const char*name);};
#include"../../src/film/film.cpp"
