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