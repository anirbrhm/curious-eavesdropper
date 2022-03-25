#include <iostream> 
#include "spins.hpp" 
#include "para.hpp"
#include <cstdlib>
#include <math.h>

using namespace std ; 

int mod(int a){
    if(a >= 0) return a ; 
    else return -a ; 
}

void advance(Spins &S){
    int Enow = S.energy() ; 
    E[0] = Enow ; 
    m[0] = S.mag() ; 

    int step = 0 ; 

    while ((mod(m[step]) < eps) || ( (1 - mod(m[step]))< eps  ) || (step < max_steps)){
        step += 1 ; 
        int i = rand()%(N+1) + 1 ;
        int j = rand()%(M+1) + 1 ; 

        S.neg(i,j) ; 
        long int Enext = S.energy() ; 

        cout << "step : " << step << ", Enow : " << Enow << ", Enext : " << Enext << ", <m> = " << S.mag() << endl ; 
        float dE = Enext - Enow ; 

        if ((float)(rand()%10)/10 < exp(-dE/T)){
            Enow = Enext ; 
        }
        else{
            S.neg(i,j) ; 
        }

        E[step] = Enow ; 
        m[step] = S.mag() ; 
    } 

    cout << "Avg mag = " << S.mag() ; 
}