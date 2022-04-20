begin
	using Plots
	using CUDA
	CUDA.versioninfo()
end

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
	
	return S
end

function EnergyGPU(S::Spins)
	E = CUDA.zeros(S.N,S.M)
	CuAr = CuArray(S.A)

	S.A[2:S.N+1,2:S.M+1]
	
	p = [(-1,0),(1,0),(0,-1),(0,1)]

	@sync begin
	@async begin
			Epart = *(CuAr[2:S.N+1,2:S.M+1], 
					CuAr[2+p[1][1]:S.N+1+p[1][1],
						2+p[1][2]:S.M+1+p[1][2]])
			E += Epart
	end
	@async begin
			Epart = *(CuAr[2:S.N+1,2:S.M+1], 
					CuAr[2+p[2][1]:S.N+1+p[2][1],
						2+p[2][2]:S.M+1+p[2][2]])
			E += Epart
	end
	@async begin
			Epart = *(CuAr[2:S.N+1,2:S.M+1], 
					CuAr[2+p[3][1]:S.N+1+p[3][1],
						2+p[3][2]:S.M+1+p[3][2]])
			E += Epart
	end
	@async begin
			Epart = *(CuAr[2:S.N+1,2:S.M+1], 
					CuAr[2+p[4][1]:S.N+1+p[4][1],
						2+p[4][2]:S.M+1+p[4][2]])
			E += Epart
	end
	end
	
	return CUDA.sum(E)
end

function EnergyGPUAlt(S::Spins)
	E = CUDA.zeros(S.N,S.M)
	CuAr = CuArray(S.A)

	S.A[2:S.N+1,2:S.M+1]
	
	p = [(-1,0),(1,0),(0,-1),(0,1)]
	@sync for i in p
			begin
			Epart = *(CuAr[2:S.N+1,2:S.M+1], 
					CuAr[2+i[1]:S.N+1+i[1],
						2+i[2]:S.M+1+i[2]])
			E += Epart
			end
	end
	return CUDA.sum(E)
end

function MagnetisationGPU(S::Spins)
	CuAr = CuArray(S.A)
	mx = (CUDA.sum(cos.(CuAr)))/ *(S.N,S.M)
	my = (CUDA.sum(sin.(CuAr)))/ *(S.N,S.M)
	return √(mx^2 + my^2)
end

function Advance(S::Spins,max_steps=10000::Int,T=5.0::Float32)
	eps = 0.001

	E = zeros(Float32,max_steps + 1)
	m = zeros(Float32,max_steps + 1)
	
	Enow = EnergyGPU(S)
	E[1] = Enow
	m[1] = MagnetisationGPU(S)

	step = 1

	for _ in 1:max_steps
		
		step += 1
		i = rand(2:S.N+1)
		j = rand(2:S.M+1)
		
		InitVal = S.A[i,j]
		S.A[i,j]= (S.A[i,j] + 2π*(rand()-0.5))%π
		Enext = EnergyGPU(S)
		dE = Enext - Enow
		
        	if rand() < exp(-dE/T)
			Enow = Enext
	        else
			S.A[i,j] = InitVal
		end

		E[step] = Enow
		m[step] = MagnetisationGPU(S)
	end
	return E,m,S.A
end

function Run(N=100::Int,M=100::Int,max_steps=10000::Int,T=5.0::Float64)	
	S = Init(N,M)
	h1 = heatmap(S.A)
	E,m,S=Advance(S,max_steps,convert(Float32,T))
	
	h2 = heatmap(S)

	l = @layout [a{0.5h};b{0.5h}]
	p1 = plot(m, linecolor = :blue, label="m")
	p2 = plot(E, lc = :red, label="E")

	a1 = plot(h1,h2,layout = l)
	a2 = plot(p1,p2,layout = l)

	plot(a2,a1,size=(600,300))
end
