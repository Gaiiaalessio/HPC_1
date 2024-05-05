#!/bin/bash
#SBATCH --job-name=HPC-OMP-WeakScaling
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1  
#SBATCH --cpus-per-task=24
#SBATCH --time=00:30:00
#SBATCH --partition=THIN
#SBATCH --exclusive
#SBATCH --exclude=fat[001-002]

module load openMPI/4.1.5/gnu/12.2.1

export OMP_PROC_BIND=spread
export OMP_PLACES=cores
export OMP_DISPLAY_ENV=true
export OMP_VERBOSE=VERBOSE

OUTPUT_CSV="weak.csv"

echo "OMP_NUM_THREADS,Problem_Size,Execution_Time" > $OUTPUT_CSV
BASE_COLS=1000
BASE_ROWS=1000

for OMP_NUM_THREADS in {1..24}; do
    export OMP_NUM_THREADS
    let cols=$BASE_COLS
    let rows=$((BASE_ROWS*OMP_NUM_THREADS))
    execution_time=$(mpirun -np 1 --map-by socket --bind-to socket ./mandelbrot $cols $rows -2.0 -1.0 1.0 1.0 255 $OMP_NUM_THREADS)

    echo "$OMP_NUM_THREADS,$cols,$rows,$execution_time" >> $OUTPUT_CSV


done
