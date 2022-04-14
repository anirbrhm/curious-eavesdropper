#include <iostream> 
#include "para.hpp"
#include <math.h>
#include <cstdlib>
#include <ctime>

using namespace std ; 

float mod(float a){
    if(a >= 0) return a ; 
    else return -a ; 
}

class Spins{
  public : 
    vector<vector<float>> A ;

    Spins(){
        A = vector<vector<float>>(N+2, vector<float>(M+2, 0.0)) ;  

        srand (static_cast <unsigned> (time(0))) ;

        for(int i=0 ; i<A.size() ; i++){
            for(int j=0 ; j<A[0].size() ; j++){
                if(i == A.size()-1 || i == 0 || j == A[0].size()-1 || j == 0) A[i][j] = 0.0 ; 
                else{
                    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ;
                    A[i][j] = r ; 
                } 
            }
        }
    }
    
    float energy(){
        float res = 0 ; 
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
    float Enow = S.energy() ; 
    E[0] = Enow ; 
    m[0] = S.mag() ; 

    int step = 0 ; 

    srand (static_cast <unsigned> (time(0)));

    while (step < max_steps){
        step += 1 ; 
        int i = rand()%(N) + 1 ;
        int j = rand()%(M) + 1 ; 

        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        S.A[i][j] = S.A[i][j] + 4*(r-0.5) ; 
        float Enext = S.energy() ; 

        cout << "step : " << step << ", Enow : " << Enow << ", Enext : " << Enext << ", <m> = " << S.mag() << endl ; 
        float dE = Enext - Enow ; 

        if(dE < 0 || rand()%10 < 10*exp(-dE/T)){
            Enow = Enext ; 
        }
        else{
            S.A[i][j] = S.A[i][j] - 4*(r-0.5) ; 
        }

        E[step] = Enow ; 
        m[step] = S.mag() ; 
    } 

    cout << "Avg mag = " << S.mag() << endl ; 
}