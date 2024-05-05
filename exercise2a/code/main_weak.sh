#!/bin/bash
#SBATCH --no-requeue
#SBATCH --job-name="hpc"
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=24
#SBATCH --exclusive
#SBATCH --time=02:00:00
#SBATCH --exclude=fat[001-002]

module load openMPI/4.1.5/gnu

mpicc -o main_weak main_weak.c

echo "processes,size,bintree,chain,flat,bcast" > weak_scaling_final.csv

for ((i=2; i<=96; i+=2)); do
    RISULTATO=$(mpiexec -np $i ./main_weak)
    echo "$RISULTATO" >> weak_scaling_final.csv
done

