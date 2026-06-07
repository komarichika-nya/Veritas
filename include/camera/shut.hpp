#pragma once
#include"conf/conf.hpp"
template<typename T>
struct shut{T begin,end;HD shut(T begin,T end);
HD T sample(T u)const;
HD T pdf()const;};
#include"../../src/camera/shut.cpp"
