#!/bin/bash
#SBATCH --no-requeue
#SBATCH --job-name="ex2"
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=24
#SBATCH --exclusive
#SBATCH --time=02:00:00
#SBATCH --exclude=fat[001-002]
#SBATCH --output=/u/dssc/galessio/notmine/HPC2/diagnostic%j.out

module load openMPI/4.1.5/gnu

mpicc -o main main.c

mpiexec -np  48 ./main
