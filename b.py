import re
vn=[]
v=[]
f=[]
with open('/home/chika/chika/lcp1/a.obj','r',encoding='utf-8') as fi:
    for l in fi:
        if l.strip().startswith('#'):continue
        if not l.strip():continue
        if l.strip().startswith('v'):
            pt=l.split(' ')
            v.append([pt[1],pt[2],pt[3]])
        elif l.strip().startswith('f'):
            fc=re.split(r' |//',l)
            f.extend(fc[1:6])
        elif l.strip().startswith('vn'):
            nor=l.split(' ')
            vn.extend(nor[1:3])
        else:continue
    print(int(len(v)))
    print(int(len(f)))
    for x in v:print(f'{x[0]} {x[1]} {x[2]}')
    for x in f:print(x)
            


