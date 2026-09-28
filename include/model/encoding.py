import trimesh,struct,numpy as np
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

input_path,output_path,mode="","","cpu"
def rd(path):
    general=False
    spp,w,h=None,None,None
    scale=()
    input_path,output_path,mode="","","cpu"
    camera=False
    pos=()
    cam,fov,lens=None,float(45),0x139
    with open(path,encoding='utf-8')as f:
        for l in f:
            l=l.strip()
            if not l:continue
            if l.startswith('#'):continue
            if l.startswith('[') and l.endswith(']'):
                general=(l=='[General]')
                camera=(l=='[Camera]')
                continue
            if general:
                tok=l.split('=')
                #print(f'{tok[0]} {tok[1]}')
                try:
                    if tok[0]=='spp':spp=int(tok[1])
                    elif tok[0]=='scale':
                        val=str(tok[1].strip('()'))
                        scale=tuple(float(v.strip())for v in val.split(','))
                    elif tok[0]=='width':w=int(tok[1])
                    elif tok[0]=='height':h=int(tok[1])
                    elif tok[0]=='input_path':input_path=str(tok[1])
                    elif tok[0]=='output_path':output_path=str(tok[1])
                    elif tok[0]=='mode':mode=str(tok[1])
                    else:raise ValueError(f'unknown parameter {tok[0]}')
                except(IndexError,ValueError)as e:
                    raise ValueError(f'{e}')from None
            if camera:
                tok=l.split('=')
                try:
                    if tok[0]=='type':
                        if str(tok[1])=='projective':cam=0x43b
                    elif tok[0]=='pos':
                        val=str(tok[1].strip('()'))
                        pos=tuple(float(v.strip())for v in val.split(','))
                        #print(f'pos:{pos} len:{len(pos)} type:{type(pos)}')
                    elif tok[0]=='lens':
                        if str(tok[1])=='dof':lens=0x139
                    elif tok[0]=='fov':fov=float(tok[1])
                    else:raise ValueError(f'unknown parameter {tok[0]}')
                except(IndexError,ValueError)as e:
                    raise ValueError(f'{e}')from None
    if spp is None or w is None or h is None:
        raise ValueError(f'missing required header field (spp/width/height)')
    if input_path=="" or output_path=="":raise FileNotFoundError(f'no such file or directory ')
    if cam is None:raise ValueError(f'missing required camera type')
    return scale,spp,w,h,input_path,output_path,mode,cam,fov,lens,pos

#header and base message test.

def load(path):
    real=os.path.expanduser(path)
    mesh=trimesh.load(real)
    if isinstance(mesh,trimesh.Scene):
        mesh=trimesh.util.concatenate(tuple(g for g in mesh.geometry.values()))
    return mesh.vertices,mesh.faces,mesh.vertex_normals

def pack(scale,spp,w,h,input_path,cam,fov,lens,campos):
    buf=bytearray()
    buf+=b'\nVES'
    buf+=struct.pack("<3f",*scale)
    buf+=struct.pack("<i",spp)
    buf+=struct.pack("<i",w)
    buf+=struct.pack("<i",h)
    pos,face,normal=load(input_path)
    buf+=struct.pack("<i",len(pos))
    buf+=struct.pack("<i",len(face)*3)
    buf+=struct.pack("<i",len(normal))
    for x,y,z in pos:buf+=struct.pack("<3f",x,y,z)
    for f in face:buf+=struct.pack("<3i",*f)
    for n in normal:buf+=struct.pack("<3f",*n)
    buf+=struct.pack("<i",cam)
    #print(f'pos:{campos} len:{len(campos)} type:{type(campos)}')
    buf+=struct.pack("<i",lens)
    buf+=struct.pack("<f",fov)
    buf+=struct.pack("<3f",*campos)
    return bytes(buf)

if __name__=="__main__":
    scale,spp,w,h,input_path,output_path,mode,cam,fov,lens,pos=rd('a.v')
    print(rd('a.v'))

    with open('/home/chika/lcp1/build/conf','wb')as f:
        f.write(pack(scale,spp,w,h,input_path,cam,fov,lens,pos))
    print('ok ^_^')

