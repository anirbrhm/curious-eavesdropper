import numpy as np
import para
from spins import Spins
import matplotlib.pyplot as plt

def advance(S: Spins,number,rank,nprocs,comm):								
    Enow = S.energy()
    para.E[0] = Enow
    para.m[0] = np.sum(S.A)/ (S.N * S.M)
    step = 0
    while(step < (para.max_steps//nprocs)):
        step += 1
        i = np.random.randint(1,S.N+1)						#Choosing random i,j in left side of processors matrix so that flipping etc does not effect calculations for left side of another processor which is receiving an array from right part of this processor's matrix
        j = np.random.randint(1,(S.M//2)+1)
        S.A[i,j] = -S.A[i,j]
        Enext = S.energy()
        sums=0
        for i in range(number):
            sums+=np.sum(S.A[:,i+1])
        dE=(Enext-Enow)/2
        if np.random.rand() < np.exp(-dE/para.T):
            Enow = Enext
        else:
            S.A[i,j] = -S.A[i,j]
        para.E[step] = Enow
        para.m[step] = sums/(S.N*S.M)
        Left=np.zeros([S.N+2,1],dtype=int)
        Right=np.zeros([S.N+2,1],dtype=int)
        if(rank>0):								#After flipping operation ends, send the column array to processor on left(rank-1) so that it has updated neighors for calculations for right part of its matrix
                Left=S.A[:,1]
                comm.send(Left,dest=(rank-1),tag=31)
        if(rank<(nprocs-1)):
                Right=comm.recv(source=(rank+1),tag=31)
                S.A[:,S.M+1]=Right
        step+=1
        i = np.random.randint(1,S.N+1)						#Same as above for right part of matrix
        j = np.random.randint((S.M//2)+1,S.M+1)
        Enow=S.energy()
        S.A[i,j] = -S.A[i,j]
        Enext = S.energy()
        sums=0
        for i in range(number):
            sums+=np.sum(S.A[:,i+1])
        dE=(Enext-Enow)/2
        if np.random.rand() < np.exp(-dE/para.T):
            Enow = Enext
        else:
            S.A[i,j] = -S.A[i,j]
        para.E[step] = Enow
        para.m[step] = sums/(S.N*S.M)
        if(rank<(nprocs-1)):
                Right=S.A[:,S.M]
                comm.send(Right,dest=(rank+1),tag=41)
        if(rank>0):
                Left=comm.recv(source=(rank-1),tag=41)
                S.A[:,0]=Left
        para.E[step] = Enow
        para.m[step] = sums/(S.N*S.M)

def display(save=True):					#Display values to be adjusted according to no of processors,steps etc
    fig = plt.figure(figsize= (3.5, 2.5))
    ax = fig.add_subplot(1,1,1)
    ax.plot(para.m, label = 'T ={}'.format(para.T))
    ax.set_xlim(0,31250)
    ax.set_xlabel('n')
    ax.set_ylabel('<M>')
    ax.set_xticks([0,15000,31250])
    ax.set_yticks([0,0.35,0.7,1])
    plt.xlabel('n')
    plt.legend(loc='center right')
    if save:
        plt.savefig('figure3.pdf')
    #plt.show()