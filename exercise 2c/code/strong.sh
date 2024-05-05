#!/bin/bash
#SBATCH --job-name=Strong-Scaling
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=24  i
#SBATCH --time=00:30:00
#SBATCH --partition=THIN
#SBATCH --exclusive
#SBATCH --exclude=fat[001-002]

module load openMPI/4.1.5/gnu/12.2.1

export OMP_PROC_BIND=spread
export OMP_PLACES=cores
export OMP_DISPLAY_ENV=true
export OMP_VERBOSE=VERBOSE

OUTPUT_CSV="omp_strong_thin.csv"

echo "OMP_NUM_THREADS,Execution_Time" > $OUTPUT_CSV

for OMP_NUM_THREADS in {1..24..1}; do
    export OMP_NUM_THREADS
    echo "Running with $OMP_NUM_THREADS OpenMP threads."

    execution_time=$(./mandelbrot 800 600 -2.0 -1.0 1.0 1.0 255 $OMP_NUM_THREADS)

    echo "$OMP_NUM_THREADS,$execution_time" >> $OUTPUT_CSV

done

