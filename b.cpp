#include<bits/stdc++.h>
using namespace std;
using u32=uint32_t;
using u64=uint64_t;
u64 qp(u64 a,u64 b,u64 m){u64 ans=1ull;a%=m;for(;b;b>>=1,a=a*a%m)(b&1)&&(ans=ans*a%m);return ans;}
u64 sum(u64 a,u64 c,u64 m,u64 x){u64 ans=0,cur=1;a%=m;for(;x;x>>=1,cur=cur*(1+a)%m,a=a*a%m)(x&1)&&(ans=(ans*a%m+cur%m));return ans;}
float cg(u64 a,u64 c,u64 m,u64 x){return float(rng(a,c,m,x))/float(4294967296.0);}
u64 rng(u64 a,u64 c,u64 m,u64 x0,int x){if(!x)return x0;u64 ai=qp(a,x,m),gs=sum(a,c,m,x);return (ai*x0+c*gs)%m;}
//string ec(u64 a,u64 c,u64 m,u64 sd){int n=s.size();string ans;ans.resize(n);int x;for(x=0;x<n;x++){u64 ks=rng(a,c,m,sd,x);ans[x]=s[x]^(char)(ks&0xff);}return ans;}
int main(){
int a=100,c=200,m=300,v=400;int x,n=1e5;double ex=0,var=0,cov=0,covv=0,s=0,sq=0;
for(x=1;x<=10;x++){double u=cg(a,c,m,v);cerr<<u<<endl;s+=u;sq+=(double)u*u;}ex=s/n,var=sq/n-ex*ex;printf("ex:%.6f var:%.6f",ex,var);
ex=var=sq=0;return 0;}
