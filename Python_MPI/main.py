import para
import fns
from spins import Spins
from init import init
from numpy import zeros
from mpi4py import MPI
import time

start=time.time()
comm=MPI.COMM_WORLD
rank=comm.Get_rank()
nprocs=comm.Get_size()
size=nprocs
width=zeros(size,dtype=int)
temp=para.M
for i in range(size):				#Getting number of columns for each processor 
    wid=int(temp/(size-i))
    width[i]=wid
    temp-=wid
S = Spins()
init(S,width[rank],rank,nprocs,comm)
fns.advance(S,width[rank],rank,nprocs,comm)
fns.display()
end=time.time()
print(end-start)