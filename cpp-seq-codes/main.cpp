#include <iostream>
#include "fns-and-classes.cpp" 
#include <chrono>

using namespace std ; 

int main(){
    auto start = chrono::high_resolution_clock::now() ;
    Spins S = Spins() ; 
    cout << S.energy() << endl ;

    advance(S) ;  

    auto stop = chrono::high_resolution_clock::now() ;
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start) ;

    cout << "The taken for the program : " << duration.count()/1000000 << " seconds" << endl ;

    return 0 ; 
}