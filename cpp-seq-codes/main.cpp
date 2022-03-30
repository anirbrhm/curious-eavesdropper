#include <iostream>
#include "fns-and-classes.cpp" 

using namespace std ; 

int main(){
    Spins S = Spins() ; 
    cout << S.energy() << endl ;

    advance(S) ;  

    return 0 ; 
}