#include<iostream>
#include<cstdio>
#include<string>
#include"conf/optional.hpp"
#include"util/mesh.hpp"
#include"util/shape.hpp"
#include"util/surface.hpp"
optional<std::string>f(bool b){if(b)return std::string("ac");return{};}
optional<std::string>f1(bool b){if(b)return std::string("ac");return{};}
int main(){return 0;}
