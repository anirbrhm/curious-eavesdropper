#include <iostream>
#include "fns-and-classes.cpp" 
#include <chrono>
#include <fstream>

using namespace std ; 

int main(){
    auto start = chrono::high_resolution_clock::now() ;
    Spins S = Spins() ; 
    cout << S.energy() << endl ;

    ofstream outAinit("Ainit.txt") ;
    for(int i=0 ; i<S.A.size() ; i++){
        for(int j=0 ; j<S.A[i].size() ; j++){
            outAinit << S.A[i][j] << "\n" ; 
        }
    }

    advance(S) ;  

    auto stop = chrono::high_resolution_clock::now() ;
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start) ;

    cout << "The taken for the program : " << duration.count()/1000000 << " seconds" << endl ;

    ofstream outm("m.txt");
    for(int i=0 ; i<m.size() ; i++) outm << m[i] << "\n" ; 

    ofstream outa("A.txt") ; 
    for(int i=0 ; i<S.A.size() ; i++){
        for(int j=0 ; j<S.A[i].size() ; j++){
            outa << S.A[i][j] << "\n" ; 
        }
    }

    return 0 ; 
}