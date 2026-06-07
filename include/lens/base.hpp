#pragma once
#include"camera/cameraSample.hpp"
#include"core/ray.hpp"
#include"camera/pose.hpp"
template<typename tp,typename T>class Lens{
public:HD Lens()=default;
HD Lens(cameraSample<T>&cs,ray<T>*r,Pose<T>&pos){static_cast<tp*>(this)->build(cs,r,pos);}};