#include <mpi.h>
#include <iostream>
#include "fns-and-classes.cpp" 
#include <chrono>
#include <fstream>

using namespace std ; 

Spins S = Spins() ; 
auto start = chrono::high_resolution_clock::now() ;

int main(int argc, char *argv[]) {
    int numprocs, myid ; 
    
    MPI_Init(&argc, &argv);  
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);  
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    cout << "Hi this is thread " << myid << endl ; 

    advance(S);  // Every MPI thread will run the parallel work

    MPI_Finalize(); 

    if(myid == 0){
        printf("Performing the final serial computation on cpu %d\n", myid);
        auto stop = chrono::high_resolution_clock::now() ;
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start) ;

        cout << "The taken for the program : " << duration.count()/1000000 << " seconds" << endl ;

        ofstream outm("m.txt");
        for(int i=0 ; i<m.size() ; i++) outm << m[i] << "\n" ; 

        ofstream outE("E.txt") ; 
        for(int i=0 ; i<E.size() ; i++) outE << E[i] << "\n" ; 
    }
 
    return 0;  
}  