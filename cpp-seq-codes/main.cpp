#include <iostream>
#include "fns.hpp" 
#include "para.hpp"
#include "spins.hpp" 

using namespace std ; 

int main(){
    Spins S = Spins() ; 
    cout << S.energy() << endl ; 

    advance(S) ; 

    return 0 ; 
}