import numpy as np
import para
from spins import Spins

def init(S: Spins,col,rank,nprocs,comm):
    S.N = para.N 
    S.M = col
    S.A = np.random.randint(0,2, (S.N+2, S.M+2)) 				# Creating random lattice of (N+2)*(width[rank]+2) where width[rank] is number of columns for process rank extra 2 columns for left and right neighbour
    S.A[0,:]=0; S.A[S.N+1,:]=0;							# Assigning 0 to all points above and below N*M lattice
    if(rank==0):								#Assigning 0 to leftmost column of process 0
        S.A[:,0]=0
    elif(rank==(nprocs-1)):							#Assigning 0 to rightmost column of last process
        S.A[:,S.M+1]=0
    boolarr1 = S.A[1:S.N + 1,1:S.M + 1] == 0 					# Susequent steps create N*M lattice of 1 and -1
    rand = np.random.rand(S.N, S.M)
    boolarr2 = rand > 0.7
    boolarr = boolarr1 & boolarr2
    S.A[1:S.N + 1,1:S.M + 1] -= boolarr
    boolarr = boolarr1 & ~boolarr2
    S.A[1:S.N + 1,1:S.M + 1] += boolarr
    if(rank<(nprocs-1)):							#Sending/receiving left and right columns between processes
        right=S.A[:,S.M]
        comm.send(right,dest=(rank+1),tag=11)
        right=comm.recv(source=(rank+1),tag=21)
        S.A[:,S.M+1]=right
    if(rank>0):
        left=comm.recv(source=(rank-1),tag=11)
        S.A[:,0]=left
        left=S.A[:,1]
        comm.send(left,dest=(rank-1),tag=21)