import para

struct Spins
	N::Int64
	M::Int64
	A::Array
end

energy(S::Spins) = sum(
+(S.A[2:para.N+2,2:para.M+2] * S.A[1:para.N+1,2:para.M+2]
, S.A[2:para.N+2,2:para.M+2] * S.A[3:para.N+3,2:para.M+2]
, S.A[2:para.N+2,2:para.M+2] * S.A[2:para.N+2,1:para.M+1]
, S.A[2:para.N+2,2:para.M+2] * S.A[2:para.N+2,3:para.M+3]))
