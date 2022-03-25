#include "para.hpp"
#include <vector>
#include <stdlib.h>

using namespace std ; 

class Spins{
  public : 
    vector<vector<int>> A ;

    Spins(){
        A = vector<vector<int>>(N+2, vector<int>(M+2, 1)) ;  

        for(int i=0 ; i<A.size() ; i++){
            for(int j=0 ; j<A[0].size() ; j++){
                if(i == A.size()-1 || i == 0 || j == A[0].size()-1 || j == 0) A[i][j] = 0 ; 
                else{
                    int random = rand()%10 ; 
                    if(random >= 7) A[i][j] = -1 ;
                } 
            }
        }
    }
    
    long int energy(){
        long int res = 0 ; 
        for(int i=0 ; i<A.size() ; i++){
            for(int j=0 ; j<A[0].size()-1 ; j++){
                res += A[i][j] * A[i][j+1] ; 
            }
        }        

        for(int i=0 ; i<A[0].size() ; i++){
            for(int j=0 ; j<A.size() ; j++){
                res += A[j][i] * A[j+1][i] ; 
            }
        }
        
        return res ; 
    }

    void neg(int i, int j){
        A[i][j] = -A[i][j] ; 

        return ; 
    }

    int mag(){
        int sum = 0 ; 
    
        for(int i=0 ; i<A.size() ; i++){
            for(int j=0 ; j<A[0].size() ; j++){
                sum += A[i][j] ; 
            }
        }

        return (sum/N)/M ; 
    }

} ; 


