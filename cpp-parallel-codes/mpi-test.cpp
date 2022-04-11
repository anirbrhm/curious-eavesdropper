#include <iostream>
#include <mpi.h>
using namespace std ; 

int main(int argc, char *argv[]) {
    int numprocs, myid ; 
    
    MPI_Init(&argc, &argv);  
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);  
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    if(myid == 0){
        cout << "Hi I am 0" << endl ; 
    }

    return 0 ; 
}