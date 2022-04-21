from numpy import zeros

N = 100				#N and M are dimensions of 2D lattice
M = 100
max_steps = 100000					#Number of steps
E = zeros(max_steps + 1) 
m = zeros(max_steps + 1)				#Magnetisation array
T = 10  
eps = 0.01
