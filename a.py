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
def sp(u1,u2):
    phi=2*np.pi*u2;r=np.sqrt(u1)
    x=r*np.cos(2*np.pi*phi)
    y=r*np.sin(2*np.pi*phi)
    z=np.sqrt(1-u1)
    return np.array([x,y,z])
ctr=np.array([0.0,3.0,0.0])
R=3.0
eps=1e-6
def hit1(wo,wi):
    oc=wo-ctr
    a,b,c=wi[0]*wi[0]+wi[1]*wi[1]+wi[2]*wi[2],2.0*(wi[0]*oc[0]+wi[1]*oc[1]+wi[2]*oc[2]),oc[0]*oc[0]+oc[1]*oc[1]+oc[2]*oc[2]-R*R
    T dt=b*b-4.0*a*c
    if dt<0:return -1e8
    T s=np.sqrt(dt)
    T s1=(-b+s)/(2*a)
    T s2=(-b-s)/(2*a)
    mx=max(s1,s2);mn=min(s1,s2)
    if mx>eps:return mx
    if mn>eps:return mn
    return -1e8

def E(wo,spp=1000):
    wo=nor(wo)
    acc=np.zero(3)
    for _ in range(spp):
        u1,u2=np.random().rand(),np.random().rand()
        wi=sp(u1,u2)
        pdf=wi[2]/np.pi if wi[2]>0 else 0.0
        if pdf<=0:continue
        f+=bsdf(wo,wi)
        acc+=f*wi[2]/pdf
    return acc/spp
print(f'E:{E(np.array([0.0,0.0,1.0]))} should approx 1')


