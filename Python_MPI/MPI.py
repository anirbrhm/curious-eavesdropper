
#All code in single file not cleaned

from mpi4py import MPI
import numpy as np
from numpy import zeros
import matplotlib.pyplot as plt
import time
start=time.time()
comm=MPI.COMM_WORLD
rank=comm.Get_rank()
nprocs=comm.Get_size()
size=nprocs
width=zeros(size,dtype=int)
N = 1000				#N and M are dimensions of 2D lattice
M = 1000
temp=M
for i in range(size):
    wid=int(temp/(size-i))
    width[i]=wid
    temp-=wid
max_steps = 10000000 					#Number of steps
E = zeros(max_steps + 1) 
m = zeros(max_steps + 1)
T = 10  
eps = 0.01 
left=zeros([N+2,1],dtype=int)
right=zeros([N+2,1],dtype=int)
class Spins:
    def __init__(self) -> None:
        self.N = None
        self.M = None
        self.A = []
    
    					# Sum of product of spin with neighbor in Up,Down,Left and Right respectively
    def energy(self):
        return (-np.sum(self.A[1:S.N+1,1:S.M+1]*self.A[0:S.N,1:S.M+1]  \
            +  self.A[1:S.N+1,1:S.M+1]*self.A[2:S.N+2,1:S.M+1] \
            +  self.A[1:S.N+1,1:S.M+1]*self.A[1:S.N+1,0:S.M] \
            +  self.A[1:S.N+1,1:S.M+1]*self.A[1:S.N+1,2:S.M+2]))

def init(S: Spins):
    S.N = N 
    S.M = width[rank]
    S.A = np.random.randint(0,2, (S.N+2, S.M+2)) 				# Creating random lattice of (N+2)*(width[rank]+2) where width[rank] is number of columns for process rank extra 2 columns for left and right neighbour
    #S.A[0,:] = 0; S.A[N + 1, :] = 0; 						# Assigning 0 to all points around N*M lattice
    #S.A[:,0] = 0; S.A[:,M + 1] = 0;
    S.A[0,:]=0; S.A[S.N+1,:]=0;
    if(rank==0):
        S.A[:,0]=0
    elif(rank==(size-1)):
        S.A[:,S.M+1]=0
    boolarr1 = S.A[1:S.N + 1,1:S.M + 1] == 0 					# Susequent steps create N*M lattice of 1 and -1
    rand = np.random.rand(S.N, S.M)
    boolarr2 = rand > 0.7
    boolarr = boolarr1 & boolarr2
    S.A[1:S.N + 1,1:S.M + 1] -= boolarr
    boolarr = boolarr1 & ~boolarr2
    S.A[1:S.N + 1,1:S.M + 1] += boolarr
    if(rank<(nprocs-1)):							#Sending/receiving left and right columns etween processes
        right=S.A[:,S.M]
        comm.send(right,dest=(rank+1),tag=11)
        right=comm.recv(source=(rank+1),tag=21)
        S.A[:,S.M+1]=right
        #print(right)
        #if(size>1):
            #comm.send(S.A[:,S.M],dest=1,tag=0)    
        #comm.Send([right,MPI.INT],dest=1)
    if(rank>0):
        #left[:,0]=0
        left=comm.recv(source=(rank-1),tag=11)
        #comm.Recv([left,MPI.INT],source=(rank-1))
        S.A[:,0]=left
        left=S.A[:,1]
        comm.send(left,dest=(rank-1),tag=21)
        #comm.Send([right,MPI.INT],dest=(rank+1))
    #else:
        #comm.recv(S.A[:,0],source=(rank-1),tag=(rank-1))
        #comm.Recv([left,MPI.INT],source=(rank-1))
def advance(S: Spins):								#Other code remains mostly same with some extra redundant things here and there. Ignore.
    Enow = S.energy()
    #Enow2=np.zeros_like(Enow)
    #Enow2=comm.allreduce(Enow,op=MPI.SUM)
    E[0] = Enow
    m[0] = np.sum(S.A)/ (S.N * S.M)

    step = 0

    #while ((abs(m[np.sum(step[:])]) < eps) or ( (1 - abs(m[np.sum(step[:])]))< eps  ) or (int(np.sum(step[:])) < max_steps) ):
    while(step < (max_steps//size)):
        step += 1
        #i = np.random.randint(1,S.N+1)
        #j = np.random.randint(1,S.M+1)
        i = np.random.randint(1,S.N+1)
        j = np.random.randint(1,(S.M//2)+1)
        S.A[i,j] = -S.A[i,j]
        Enext = S.energy()
        #Enext2=np.zeros_like(Enext)
        #Enext2=comm.allreduce(Enext,op=MPI.SUM)
        sums=0
        for i in range(width[rank]):
            sums+=np.sum(S.A[:,i+1])
        #m2=comm.allreduce(sums,op=MPI.SUM)
        #m2=0
        #print("step, Enow, Enext, <m> = ", step, Enow, Enext, sums/(S.N * S.M) )
        #dE = Enext2 - Enow2
        dE=(Enext-Enow)/2
        if np.random.rand() < np.exp(-dE/T):
            Enow = Enext
        else:
            S.A[i,j] = -S.A[i,j]
        E[step] = Enow
        m[step] = sums/(S.N*S.M)
        Left=zeros([N+2,1],dtype=int)
        Right=zeros([N+2,1],dtype=int)
        if(rank>0):
                Left=S.A[:,1]
                comm.send(Left,dest=(rank-1),tag=31)
        if(rank<(nprocs-1)):
                Right=comm.recv(source=(rank+1),tag=31)
                S.A[:,S.M+1]=Right
        step+=1
        i = np.random.randint(1,S.N+1)
        j = np.random.randint((S.M//2)+1,S.M+1)
        Enow=S.energy()
        S.A[i,j] = -S.A[i,j]
        Enext = S.energy()
        sums=0
        for i in range(width[rank]):
            sums+=np.sum(S.A[:,i+1])
        dE=(Enext-Enow)/2
        if np.random.rand() < np.exp(-dE/T):
            Enow = Enext
        else:
            S.A[i,j] = -S.A[i,j]
        E[step] = Enow
        m[step] = sums/(S.N*S.M)
        if(rank<(nprocs-1)):
                Right=S.A[:,S.M]
                comm.send(Right,dest=(rank+1),tag=41)
        if(rank>0):
                Left=comm.recv(source=(rank-1),tag=41)
                S.A[:,0]=Left
        #const=np.sum(step[:])
        #print(const)
        #step2=np.zeros_like(step)
        #step2=comm.reduce(step,root=rank,op=MPI.SUM)
        E[step] = Enow
        m[step] = sums/(S.N*S.M)
    #print("Avg mag = ", np.sum(S.A)/(S.N * S.M))
    #print(S.A)
# def save_array():
#     np.save('E_4_T_10.npy', para.E)
#     np.save('m_4_T_10.npy', para.m)

def display(save=True):
    fig = plt.figure(figsize= (3.5, 2.5))
    ax = fig.add_subplot(1,1,1)
    ax.plot(m, label = 'T ={}'.format(T))
    ax.set_xlim(0,312500)
    ax.set_xlabel('n')
    ax.set_ylabel('<M>')
    ax.set_xticks([0,150000,312500])
    ax.set_yticks([0,0.35,0.7,1])
    plt.xlabel('n')
    plt.legend(loc='center right')
    if save:
        plt.savefig('figure2.pdf')
    #plt.show()
#print("3")
S = Spins()
#print(S.A)
init(S)
#print(S.A)
#print(rank)
#print(S.energy())
advance(S)
display()
end=time.time()
print(end-start)