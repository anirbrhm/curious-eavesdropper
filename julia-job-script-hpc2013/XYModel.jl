using Plots
using FLoops
using BenchmarkTools
using Octavian

struct Spins
	N::Int32
	M::Int32
	A::Array{Float32}
end

function Init(N = 100::Int,M = 100::Int)
	S = Spins(N, M, rand(Float32, N+2 ,M+2)*2π .- π)
	S.A[1,:] .= 0.0
	S.A[N + 2, :] .= 0.0
	S.A[:,1] .= 0.0
	S.A[:,M + 2] .= 0.0

	boolarr1 = S.A[2:N + 1,2:M + 1] .== 0.0
	temparr = rand(Float32,N,M)
	boolarr2 = temparr .> 0.7
	boolarr = boolarr1 .& boolarr2
	S.A[2:N + 1,2:M + 1] -= boolarr
	boolarr = boolarr1 .& .~boolarr2
	S.A[2:N + 1,2:M + 1] += boolarr

	return S
end

function EnergyParallel(S::Spins)
	E = zeros(Float32,S.N+2,S.M+2)
	p = [(-1,0),(1,0),(0,-1),(0,1)]
	@floop ThreadedEx(basesize = 1, nestlevel=Val(:inf), simd = true) for i in p
			Epart = matmul(S.A[2:S.N+1,2:S.M+1],
					S.A[2+i[1]:S.N+1+i[1],
						2+i[2]:S.M+1+i[2]])
			@reduce(E += Epart)
	end
	return sum(E)
end

function Magnetisation(S::Spins)
	mx = sum(cos.(S.A))/ *(S.N,S.M)
	my = sum(sin.(S.A))/ *(S.N,S.M)
	return √(mx^2 + my^2)
end

function Advance(S::Spins,max_steps=10000::Int,T=5.0::Float32)
	eps = 0.001

	E = zeros(Float32,max_steps + 1)
	m = zeros(Float32,max_steps + 1)

	Enow = EnergyParallel(S)
	E[1] = Enow
	m[1] = Magnetisation(S)

	step = 1

	#while *(step<max_steps, m[step] > eps, 1 - m[step] > eps)
	for _ in 1:max_steps

		step += 1
		i = rand(2:S.N+1)
		j = rand(2:S.M+1)

		InitVal = S.A[i,j]

		S.A[i,j]= (S.A[i,j] + 2π*(rand()-0.5))%π

		Enext = EnergyParallel(S)
		dE = Enext - Enow

        if dE < 0
			Enow = Enext
		elseif	rand() < exp(-dE/T)
            Enow = Enext
        else
            S.A[i,j] = InitVal
		end

		E[step] = Enow
		#m[step] = Magnetisation(S)
	end
	return E,m,S.A
end

function Run(N=100::Int,M=100::Int,max_steps=10000::Int,T=Float32(5.0)::Float32)
	S = Init(N,M)
	h1 = heatmap(S.A)
	E,m,S=Advance(S,max_steps,T)

	"""l = @layout [a{0.6h};b{0.5h}]
	p1 = plot(m, linecolor = :blue, label="m")
	p2 = plot(E, lc = :red, label="E")
	plot(p1,p2,layout = l)"""

	h2 = heatmap(S)
	savefig(plot(h1,h2), "./Out")
end

@time Run(100,100,10)
println(Threads.nthreads())
