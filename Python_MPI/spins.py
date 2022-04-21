import numpy as np
import para
class Spins:
    def __init__(self) -> None:
        self.N = None
        self.M = None
        self.A = []
    
    					# Sum of product of spin with neighbor in Up,Down,Left and Right respectively
    def energy(self):
        return (-np.sum(self.A[1:self.N+1,1:self.M+1]*self.A[0:self.N,1:self.M+1]  \
            +  self.A[1:self.N+1,1:self.M+1]*self.A[2:self.N+2,1:self.M+1] \
            +  self.A[1:self.N+1,1:self.M+1]*self.A[1:self.N+1,0:self.M] \
            +  self.A[1:self.N+1,1:self.M+1]*self.A[1:self.N+1,2:self.M+2]))