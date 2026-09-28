import configparser
import trimesh,struct,numpy as np
from dataclasses import dataclass,field,fields
import os

# header 4 byte     string
#scale  4 byte      float*3
# spp   4 byte      int
# w     4 byte      int
# h     4 byte      int
#len(pos) 4 byte    int
#len(face) 4 byte   int
#len(normal) 4 byte int

#--- pos data ---
# 4 byte      float*3
#--- face data ---
# 4 byte        int
#--- normal data ---
# 4 byte        float*3

#--camera info--
# camera type  4 byte   int
# camera lens       4 byte  int
# camera fov       4 byte   float
# camera position  4 byte   float*3

magic=b'\nVES'
def vec3(s):
    v=tuple(float(x)for x in s.strip().strip('()').split(','))
    if len(v)!=3:raise ValueError(f'the format must be (x,y,z), found {s!r}')
    return v
def enum(**table):
    def parse(s):
        s=s.strip()
        if s not in table:
            raise ValueError(f'value error {s!r}, supported {list(table)}')
        return table[s]
    return parse

CAMERAS=enum(projective=0x43b)
LENS=enum(dof=0x139)

req=object()
def F(sec,parse,fmt=None,dft=req,key=None):
    return field(default=None if dft is req else dft,
                 metadata=dict(sec=sec,parse=parse,fmt=fmt,key=key,req=dft is req))
@dataclass
class config:
    #[General]
    scale:tuple=F('General',vec3,'<3f',dft=(1.0,1.0,1.0))
    spp:int=F('General',int,'<i')
    width:int=F('General',int,'<i')
    height:int=F('General',int,'<i')
    input_path:str=F('General',str)
    output_path:str=F('General',str)
    mode:str=F('General',str,dft='cpu')
    #[Camera]
    camera:int=F('Camera',CAMERAS,'<i',key='type')
    lens:int=F('Camera',LENS,'<i',dft=0x139)
    fov:float=F('Camera',float,'<f',dft=45.0)
    pos:tuple=F('Camera',vec3,'<3f',dft=(0,0,0))

def rd(path):
    cp=configparser.ConfigParser(comment_prefixes=('#',),inline_comment_prefixes=('#',),interpolation=None)
    cp.optionxform=str
    with open(path,encoding='utf-8')as f:
        cp.read_file(f)
    known,kwargs={},{}
    for fd in fields(config):
        m=fd.metadata
        #print(m)
        sec,key=m['sec'],m['key'] or fd.name
        #print(sec,key)
        known.setdefault(sec,set()).add(key)
        #print(known)
        raw=cp.get(sec,key,fallback=None)
        if raw is None:
            if m['req']:raise ValueError(f'[{sec}] missing required {key}')
            continue
        try:
            kwargs[fd.name]=m['parse'](raw)
        except ValueError as e:
            raise ValueError(f'[{sec}] {key}: {e}')from None
    #print(known)
    for sec in cp.sections():
        extra=set(cp[sec])-known.get(sec,set())
        #print(extra)
        if extra:raise ValueError(f'[{sec}] unknown {sorted(extra)}')
    return config(**kwargs)

def load(path):
    mesh=trimesh.load(os.path.expanduser(path))
    if isinstance(mesh,trimesh.Scene):
        mesh=trimesh.util.concatenate(mesh.geometry.values())
    return mesh.vertices,mesh.faces,mesh.vertex_normals
def pack_sec(cfg,sec):
    out=bytearray()
    for fd in fields(cfg):
        m=fd.metadata
        if m['sec']!=sec or m['fmt']is None:continue
        v=getattr(cfg,fd.name)
        print(v)
        out+=struct.pack(m['fmt'],*(v if isinstance(v,tuple)else(v,)))
    return bytes(out)
def pack(cfg):
    pos,face,normal=load(cfg.input_path)
    return b''.join([magic,pack_sec(cfg,'General'),struct.pack('<3i',len(pos),face.size,len(normal)),
                     pos.astype('<f4').tobytes(),face.astype('<i4').tobytes(),normal.astype('<f4').tobytes(),
                     pack_sec(cfg,'Camera')])
if __name__=='__main__':
    cfg=rd('a.v')
    with open('/home/chika/lcp1/build/conf','wb')as f:
        f.write(pack(cfg))
    print(f'\033[1;35mok ^_^\033[0m')
