# H-max transform Accelerated with PyCUDA
# Yunfan Zhang
# yf.g.zhang@gmail.com
# 3/09/2017
# Usage: python GameOfLife.py n n_iter
# where n is the board size and n_iter the number of iterations
import pycuda.driver as cuda
import pycuda.tools
import pycuda.autoinit
import pycuda.gpuarray as gpuarray
from pycuda.compiler import SourceModule
import sys
import numpy as np
from pylab import cm as cm
import matplotlib.pyplot as plt

def random_init(n):
    #np.random.seed(100)
    M = np.zeros((n,n,n)).astype(np.int32)
    for k in range(n):
      for j in range(n):
          for i in range(n):
              M[k,j,i] = np.int32(np.random.randint(2))
    return M
mod = SourceModule("""
  __global__ void step(float *C, float *M, bool *Mask, bool *maxima, float h)
  {
    bool ismax = true;
    int n_x = blockDim.x*gridDim.x;
    int n_y = blockDim.y*gridDim.y;
    int i = threadIdx.x + blockDim.x*blockIdx.x;
    int j = threadIdx.y + blockDim.y*blockIdx.y;
    int k = threadIdx.z + blockDim.z*blockIdx.z;
    int threadId = k*n_x*n_y + j*n_x + i;
    int i_left; int i_right; int j_down; int j_up; int k_down; int k_up;
    //Mirror boundary condition
    if(i==0) {i_left=i;} else {i_left=i-1;}   
    if(i==n_x-1) {i_right=i;} else {i_right=i+1;}
    if(j==0) {j_down=j;} else {j_down=j-1;}
    if(j==n_x-1) {j_up=j;} else {j_up=j+1;}
    if(k==0) {k_down=k;} else {k_down=k-1;}
    if(k==n_x-1) {k_up=k;} else {k_up=k+1;}
    int neighbors[6] = {k*n_x*n_x+j*n_x+i_left, k*n_x*n_x+j_down*n_x+i, k*n_x*n_x+j*n_x+i_right,
           k*n_x*n_x+j_up*n_x+i, k_down*n_x*n_x+j*n_x+i, k_up*n_x*n_x+j*n_x+i};
    int ne;

    if (!Mask[threadId]) {ismax = false;}
    else
    {
      for (int ni=0; ni<6; ni++) 
      {
        ne = neighbors[ni];
        if (C[threadId]<C[ne]) {ismax = false;}
      }
    }
    maxima[threadId] = ismax;
    __syncthreads();

    if (!Mask[threadId])
    {
      for (int ni=0; ni<6; ni++) 
      {
        ne = neighbors[ni];
        if (maxima[ne] == true && C[threadId] >= C[ne] - h) 
        {
          M[threadId] = (C[threadId]<C[ne]) ? C[ne] : C[threadId];
        }
      }
    }

  }
""")

def h_transform(filename=None, arr=None, mask=None, maxima=None, h=0.7, n_iter=50, n_block=8):
  if filename is not None:
    file = np.load(filename)
    arr = file['arr']; mask = file['mask']
  if arr is None:
    raise Exception('No input specified!')
    # arr = random_init(50)
  if mask is None:
    mask = arr > 0
  if maxima is None:
    maxima = arr > 0
  arr = arr.astype(np.float32)
  M = arr.copy()
  n = arr.shape[0]
  n_grid = int(n/n_block)
  n = n_block*n_grid
  func = mod.get_function("step")
  
  C_gpu = gpuarray.to_gpu( arr )
  M_gpu = gpuarray.to_gpu( M )
  mask_gpu = gpuarray.to_gpu( mask )
  max_gpu = gpuarray.to_gpu( maxima )
  h_gpu = gpuarray.to_gpu(np.array(0.7, dtype=np.float32))
  print(h_gpu.get())

  for k in range(n_iter):
    func(C_gpu,M_gpu,mask_gpu, max_gpu, h_gpu, block=(n_block,n_block,n_block),grid=(n_grid,n_grid,n_grid))
    C_gpu, M_gpu = M_gpu, C_gpu
  transformed = M_gpu.get()
  return transformed

if __name__=='__main__':
  n = int(sys.argv[1])
  n_iter = int(sys.argv[2])
  M = h_transform()
  print(M.shape)
# print("%d live cells after %d iterations" %(np.sum(C_gpu.get()),n_iter))
# fig = plt.figure(figsize=(12,12))
# ax = fig.add_subplot(111)
# fig.suptitle("Conway's Game of Life Accelerated with PyCUDA")
# ax.set_title('Number of Iterations = %d'%(n_iter))
# myobj = plt.imshow(C_gpu.get()[8],origin='lower',cmap='Greys',  interpolation='nearest',vmin=0, vmax=1)
# plt.pause(.01)
# plt.draw()
# m = 0
# while m <= n_iter:
#     m += 1
#     func(C_gpu,M_gpu,block=(n_block,n_block,n_block),grid=(n_grid,n_grid,n_grid))
#     C_gpu, M_gpu = M_gpu, C_gpu
#     myobj.set_data(C_gpu.get()[8])
#     ax.set_title('Number of Iterations = %d'%(m))
#     plt.pause(.01)
#     plt.draw()