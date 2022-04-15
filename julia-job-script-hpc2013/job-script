#!/bin/bash
#PBS -N XYModel
#PBS -q mini
#PBS -l nodes=1:ppn=20
#PBS -j oe
cd $PBS_O_WORKDIR
cat $PBS_NODEFILE > pbs_nodes
export I_MPI_FABRICS=shm:dapl
export I_MPI_MPD_TMPDIR=/scratch/rnarwar
echo "Starting run at: `date`" > $PBS_O_WORKDIR/RUN_LOG
source /opt/software/intel/initpaths intel64
PATHEXE=/home/rnarwar/Julia/julia-1.7.2/bin/julia
export JULIA_NUM_THREADS=auto
export JULIA_CPU_THREADS=20
INPUT=/home/rnarwar/XYModel.jl
mpirun -n 20 $PATHEXE <$INPUT
