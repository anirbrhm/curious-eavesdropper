#include "para.h"
#include <vector>
#include <stdlib.h>

using namespace std ; 

class Spins{
    
    int n, m ; 

    Spins(){
        this->n = N ;
        this->m = M ; 
        vector<vector<int>> A(this->n+2, vector<int>(this->m+2, 1)) ; 

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
        

        
         
    }
} ; 


