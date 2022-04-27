#include <iostream>
#include "fns-and-classes.cpp" 
#include <chrono>
#include <fstream>

using namespace std ; 

int main(){
    auto start = chrono::high_resolution_clock::now() ;
    Spins S = Spins() ; 
    cout << S.energy() << endl ;

    advance(S) ;  

    auto stop = chrono::high_resolution_clock::now() ;
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start) ;

    cout << "The taken for the program : " << duration.count()/1000000 << " seconds" << endl ;

    ofstream outm("m.txt") ; 
    for(int i=0 ; i<m.size() ; i++) outm << m[i] << "\n" ; 

    ofstream outE("E.txt") ; 
    for(int i=0 ; i<E.size() ; i++) outE << E[i] << "\n" ; 

    return 0 ; 
}