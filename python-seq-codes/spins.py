import numpy as np
import para
class Spins:
    def __init__(self) -> None:
        self.N = None
        self.M = None
        self.A = []

    # def init(self, N: int, M: int):
    #     self.N = N
    #     self.M = M
    #     self.A = 
    def energy(self):
        return (-np.sum(self.A[1:para.N+1,1:para.M+1]*self.A[0:para.N,1:para.M+1]  \
            +  self.A[1:para.N+1,1:para.M+1]*self.A[2:para.N+2,1:para.M+1] \
            +  self.A[1:para.N+1,1:para.M+1]*self.A[1:para.N+1,0:para.M] \
            +  self.A[1:para.N+1,1:para.M+1]*self.A[1:para.N+1,2:para.M+2]))
