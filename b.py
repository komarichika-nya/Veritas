import numpy as np
import sys
np.random.seed(40)
mat=np.random.randn(3,3).astype(np.float32)*5
np.savetxt('a.in',mat,fmt='%.7g',delimiter=' ')
det=np.linalg.det(mat)
inv=np.linalg.inv(mat)
np.savetxt('a.out',[det],fmt='%.7g',delimiter=' ')


