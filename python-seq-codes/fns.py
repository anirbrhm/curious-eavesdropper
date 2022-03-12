import numpy as np
import para
from spins import Spins
import matplotlib.pyplot as plt


def advance(S: Spins):
    Enow = S.energy()
    para.E[0] = Enow
    para.m[0] = np.sum(S.A)/ (S.N * S.M)

    step = 0

    while ((abs(para.m[step]) < para.eps) or ( (1 - abs(para.m[step]))< para.eps  ) or (step < para.max_steps) ):
        step += 1
        i = np.random.randint(1,S.N+1)
        j = np.random.randint(1,S.M+1)
        S.A[i,j] = -S.A[i,j]
        Enext = S.energy()
        print("step, Enow, Enext, <m> = ", step, Enow, Enext, np.sum(S.A)/(S.N * S.M) )
        dE = Enext - Enow
        if np.random.rand() < np.exp(-dE/para.T):
            Enow = Enext
        else:
            S.A[i,j] = -S.A[i,j]
        para.E[step] = Enow
        para.m[step] = np.sum(S.A)/ (S.N * S.M)
    print("Avg mag = ", np.sum(S.A)/(S.N * S.M))
    print(S.A)
# def save_array():
#     np.save('E_4_T_10.npy', para.E)
#     np.save('m_4_T_10.npy', para.m)

def display(save=False):
    fig = plt.figure(figsize= (3.5, 2.5))
    ax = fig.add_subplot(1,1,1)
    ax.plot(para.m, label = 'T ={}'.format(para.T))
    ax.set_xlim(0,100000)
    ax.set_xlabel('n')
    ax.set_ylabel('<M>')
    ax.set_xticks([0,50000,100000])
    ax.set_yticks([0,0.35,0.7,1])
    plt.xlabel('n')
    plt.legend(loc='center right')
    if save:
        plt.savefig('PT_m_vs_n.pdf')
    plt.show()