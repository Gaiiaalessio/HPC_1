#!/bin/bash
#SBATCH --job-name=HPC
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=24
#SBATCH --time=02:00:00
#SBATCH --partition THIN
#SBATCH --exclude=fat[001-002]

module load openMPI/4.1.5/gnu/12.2.1

echo "Processes,Latency" > scatterfix0.csv

# Numero di ripetizioni per ottenere una media
repetitions=10000

# Dimensione fissa del messaggio
size=2

# Ciclo esterno per il numero di processori
for processes in {2..48..2}
do
    # Esegui osu_bcast con numero di processi, dimensione fissa e numero di ripetizioni su due nodi
    result_scatter=$(mpirun --map-by core -np $processes --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_scatter_algorithm 0 osu_scatter -m $size -x $repetitions -i $repetitions | tail -n 1 | awk '{print $2}')

    # Scrivi i risultati nel file CSV
    echo "$processes,$result_scatter" >> scatterfix0.csv
done

