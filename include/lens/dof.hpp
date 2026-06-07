#pragma once
#include"camera/cameraSample.hpp"
#include"core/ray.hpp"
#include"camera/pose.hpp"
template<typename T>
class DOF:public Lens<DOF<T>,T>{
public:T dis,rds;
HD DOF(T dis,T rds);
HD void build(cameraSample<T>&cs,ray<T>*r,const Pose<T>&pos,const vec3<T,V>&dir)const;
HD void sp(T*dx,T*dy,cameraSample<T>&cs)const;};
#include"../../src/lens/dof.cpp"
