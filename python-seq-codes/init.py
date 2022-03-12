import numpy as np
import para
from spins import Spins

def init(S: Spins):
    S.N = para.N
    S.M = para.M
    S.A = np.random.randint(0,2, (para.N+2, para.M+2))
    S.A[0,:] = 0; S.A[para.N + 1, :] = 0
    S.A[:,0] = 0; S.A[:,para.M + 1] = 0
    boolarr1 = S.A[1:para.N + 1,1:para.M + 1] == 0
    rand = np.random.rand(para.N, para.M)
    boolarr2 = rand > 0.7
    boolarr = boolarr1 & boolarr2
    S.A[1:para.N + 1,1:para.M + 1] -= boolarr
    boolarr = boolarr1 & ~boolarr2
    S.A[1:para.N + 1,1:para.M + 1] += boolarr
