from PIL import Image
import numpy as np
im=np.array(Image.open('c.ppm'))
m=np.mean(im,axis=0)
print(m/255)
#print(f'{np.mean(im,axis=0)} {np.mean(im,axis=1)} {np.mean(im,axis=2)}')

