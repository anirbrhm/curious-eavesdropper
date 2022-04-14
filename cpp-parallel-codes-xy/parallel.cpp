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

    if(myid == 0){
        ofstream outAinit("Ainit.txt") ;
        for(int i=0 ; i<S.A.size() ; i++){
            for(int j=0 ; j<S.A[i].size() ; j++){
                outAinit << S.A[i][j] << "\n" ; 
            }
        }
    }

    advance(S);  // Every MPI thread will run the parallel work

    MPI_Finalize(); 

    if(myid == 0){
        printf("Performing the final serial computation on cpu %d\n", myid);
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
    }
 
    return 0;  
}  