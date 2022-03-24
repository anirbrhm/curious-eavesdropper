#include "para.h"
#include <vector>
#include <stdlib.h>

using namespace std ; 

class Spins{
    
    int n, m ; 
    vector<vector<int>> A ; 

    Spins(){
        this->n = N ;
        this->m = M ;  

        A = vector<vector<int>>(n+2, vector<int>(m+2, 1)) ;  

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
        for(int i=0 ; i<n+2 ; i++){
            for(int j=0 ; j<m+1 ; j++){
                res += A[i][j] * A[i][j+1] ; 
            }
        }        

        for(int i=0 ; i<m+2 ; i++){
            for(int j=0 ; j<n+1 ; j++){
                res += A[j][i] * A[j+1][i] ; 
            }
        }
        
        return res ; 
    }
} ; 


