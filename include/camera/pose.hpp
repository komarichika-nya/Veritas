#pragma once
#include"core/vec.hpp"
#include"core/quat.hpp"
#include"core/mat.hpp"
#include"conf/conf.hpp"
template<typename T>struct quat;
template<typename T>struct Pose{vec3<T,P>p;vec3<T,V>f,u,r;
HD Pose();
HD Pose(const vec3<T,P>&p1,const vec3<T,V>&f1,const vec3<T,V>&u1,const vec3<T,V>&r1);
HD static Pose<T>init();
HD static Pose<T>look(const vec3<T,P>&pos,const vec3<T,P>&to,const vec3<T,V>&wd);
HD void axis(const vec3<T,V>&v,T ang);
HD void mv(const vec3<T,V>&v);
HD void mv(const vec3<T,P>&v);
HD void trs_fps(T dx,T dy,T dz);
HD void rot_fps(T th,T phi,const vec3<T,V>&wd);
HD void orbit(const vec3<T,P>&ctr,const vec3<T,V>&a,const T ang);
HD void at(const vec3<T,P>&t,const vec3<T,V>&wd);
HD void clean();
HD void apply(const mat34<T>&m);
HD quat<T>to_quat();
HD static Pose<T>to_pos(const quat<T>&q,const vec3<T,P>&pos);};
#include"../../src/camera/pose.cpp"
