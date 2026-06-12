#include"conf/optional.hpp"
#include<iostream>
#include<cstdio>
#include<string>
optional<std::string>f(bool b){if(b)return std::string("ac");return{};}
optional<std::string>f1(bool b){if(b)return std::string("ac");return{};}
int main(){auto v=f(0),v1=f1(1);if(v.has_val)std::cout<<"v"<<v.val()<<std::endl;if(v1.has_val)std::cout<<"v1"<<v1.val()<<std::endl;
std::cout<<v1->size()<<std::endl;}
