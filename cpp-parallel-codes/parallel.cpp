#include <mpi.h>
#include <iostream> 
#include <algorithm>
#include <chrono>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <ctime>

using namespace std ; 

int N = 100 ; 
int M = 100 ;  
int max_steps = 50000 ; 

std::vector<int> E(max_steps + 1, 0) ; 
std::vector<float> m(max_steps + 1, 0) ;

float T = 10 ; 
float eps = 0.01 ; 

float mod(float a){
    if(a >= 0) return a ; 
    else return -a ; 
}

class Spins{
  public : 
    vector<vector<int>> A ;

    Spins(){
        A = vector<vector<int>>(N+2, vector<int>(M+2, 1)) ;  

        srand((unsigned) time(0));

        for(int i=0 ; i<A.size() ; i++){
            for(int j=0 ; j<A[0].size() ; j++){
                if(i == A.size()-1 || i == 0 || j == A[0].size()-1 || j == 0) A[i][j] = 0 ; 
                else{
                    int random = rand()%10 + 1 ;  
                    if(random > 7) A[i][j] = -1 ;
                } 
            }
        }
    }
    
    int energy(){
        int res = 0 ; 
        for(int i=0 ; i<A.size() ; i++){
            for(int j=0 ; j<A[0].size()-1 ; j++){
                res += A[i][j] * A[i][j+1] ; 
            }
        }        

        for(int i=0 ; i<A[0].size() ; i++){
            for(int j=0 ; j<A.size()-1 ; j++){
                res += A[j][i] * A[j+1][i] ; 
            }
        }
        
        return res/2 ; 
    }

    void neg(int i, int j){
        this->A[i][j] = -this->A[i][j] ; 

        return ; 
    }

    float mag(){
        float sum = 0 ; 
    
        for(int i=0 ; i<A.size() ; i++){
            for(int j=0 ; j<A[0].size() ; j++){
                sum += A[i][j] ; 
            }
        }

        return (sum/N)/M ; 
    }

} ;

void advance(Spins &S){
    int Enow = S.energy() ; 
    E[0] = Enow ; 
    m[0] = S.mag() ; 

    int step = 0 ; 

    while (step < max_steps){
        step += 1 ; 
        int i = rand()%(N) + 1 ;
        int j = rand()%(M) + 1 ; 

        S.neg(i,j) ; 
        int Enext = S.energy() ; 

        cout << "step : " << step << ", Enow : " << Enow << ", Enext : " << Enext << ", <m> = " << S.mag() << endl ; 
        float dE = Enext - Enow ; 

        if(dE < 0 || rand()%10 < 10*exp(-dE/T)){
            Enow = Enext ; 
        }
        else{
            S.neg(i,j) ; 
        }

        E[step] = Enow ; 
        m[step] = S.mag() ; 
    } 

    cout << "Avg mag = " << S.mag() << endl ; 
}

Spins S = Spins() ; 

auto start = chrono::high_resolution_clock::now() ;

int main(int argc, char *argv[]) {
    int numprocs, myid ; 
    
    MPI_Init(&argc, &argv);  
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);  
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    if (myid == 0) { // Do the serial part on a single MPI thread
        printf("Performing serial computation on cpu %d\n", myid);

        int res = 0 ; 
        for(int i=0 ; i<S.A.size() ; i++){
            for(int j=0 ; j<S.A[0].size()-1 ; j++){
                res += S.A[i][j] * S.A[i][j+1] ; 
            }
        }        

        for(int i=0 ; i<S.A[0].size() ; i++){
            for(int j=0 ; j<S.A.size()-1 ; j++){
                res += S.A[j][i] * S.A[j+1][i] ; 
            }
        }

        cout << res/2 << endl ;
    }

    advance(S);  // Every MPI thread will run the parallel work


    if (myid == 0) { // Do the final serial part on a single MPI thread
        printf("Performing the final serial computation on cpu %d\n", myid);
        auto stop = chrono::high_resolution_clock::now() ;
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start) ;

        cout << "The taken for the program : " << duration.count()/1000000 << " seconds" << endl ;
    }

    MPI_Finalize();  
    return 0;  
}  