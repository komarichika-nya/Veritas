import os,sys
import numpy as np
from PIL import Image
def nor(v):v=np.asarray(v,float);return v/np.linalg.norm(v)
def cs(a,b):return np.cross(a,b)
def vndf(i,a,u1,u2):
    vh=nor([i[0]*a,i[1]*a,i[2]])
    sq=vh[0]**2+vh[1]**2
    v1=np.array([-vh[1],vh[0],0])/np.sqrt(sq)if sq>0 else np.array([1,0,0])
    v2=cs(vh,v1)
    r=np.sqrt(u1)
    phi=2*np.pi*u2
    t1=r*np.cos(phi)
    t2=r*np.sin(phi)
    s=0.5*(1+vh[2])
    t2=(1-s)*np.sqrt(max(1-t1*t1,0))+s*t2
    nh=t1*v1+t2*v2+np.sqrt(max(0,1-t1*t1-t2*t2))*vh
    return nor([nh[0]*a,nh[1]*a,max(0,nh[2])])
i=[np.clip(np.random.rand(),-1,1),np.clip(np.random.rand(),-1,1),np.clip(np.random.rand(),-1,1)]
for n in [nor([0,0,1]),nor([0,1,0]),nor([0.7,0.7,0]),nor([0.2,0.9,0.3])]:
    tt=nor(cs(np.array([1,0,0]),n))if np.abs(n[2])>0.9999 else nor(cs(np.array([0,0,1]),n))
    bb=cs(n,tt);il=nor(np.array([np.dot(i,tt),np.dot(i,bb),np.dot(i,n)]))
    hs=[vndf(il,0.0223,np.random.rand(),np.random.rand())for _ in range(2000)]
    wd=[h[0]*tt+h[1]*bb+h[2]*n for h in hs]
    hm=nor(np.mean(wd,0))
    print(f'i={i} hm={hm} n={n}')
    print()
    ang=np.degrees(np.arccos(np.clip(np.dot(hm,n),-1,1)))
    print(f'n={np.round(n,2)} mean={np.round(hm,2)} ang={ang:5.1f}')

