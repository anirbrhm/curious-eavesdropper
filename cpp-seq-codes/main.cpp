#include <iostream>
#include "fns-and-classes.cpp" 

using namespace std ; 

int main(){
    Spins S = Spins() ; 
    cout << S.energy() << endl ;

    advance(S) ;  

    cout << "The magnetisation is : " << S.mag() << endl ; 

    return 0 ; 
}