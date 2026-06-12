from PIL import Image
import numpy as np
im=np.array(Image.open('b.ppm'))
a=np.mean(im)/255.0
print(f'{a} should approx 1')

