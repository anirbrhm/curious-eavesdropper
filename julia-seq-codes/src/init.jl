include("source.jl")

function init(S::Spins)
    S.N = para.N
    S.M = para.M
	
    S.A = rand(0,2, (para.N+2, para.M+2))
    S.A[1,:] = 0 
	  S.A[para.N + 1, :] = 0
    S.A[:,1] = 0 
	  S.A[:,para.M + 1] = 0
	
    boolarr1 = S.A[2:para.N + 2,2:para.M + 2] == 0
    rand = rand(para.N, para.M)
    boolarr2 = rand > 0.7
    boolarr = boolarr1 & boolarr2
    S.A[2:para.N + 2,2:para.M + 2] -= boolarr
    boolarr = boolarr1 & ~boolarr2
    S.A[2:para.N + 2,2:para.M + 2] += boolarr
end
