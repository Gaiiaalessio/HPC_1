#!/bin/sh
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=128
#SBATCH --time=01:00:00
#SBATCH --partition EPYC

module load openMPI/4.1.5/gnu/12.2.1

echo "Processes,Size,Latency" > processes_size_map_by_core_1_epyc_power_2.csv

# Numero di ripetizioni per ottenere una media
repetitions=25000

# Ciclo esterno per il numero di processori
for processes_power in 8
do
    processes=$((2**processes_power))
    # Ciclo interno per la dimensione del messaggio da 2^1 a 2^20
    for size_power in {1..20}
    do
        # Calcola la dimensione come 2 elevato alla potenza corrente
        size=$((2**size_power))

        # Esegui osu_bcast con numero di processi, dimensione fissa e numero di ripetizioni su due nodi
        result_bcast=$(mpirun --map-by core -np $processes --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_bcast_algorithm 1 osu_bcast -m $size -x $repetitions -i $repetitions | tail -n 1 | awk '{print $2}')

        # Scrivi i risultati nel file CSV
        echo "$processes,$size,$result_bcast" >> processes_size_map_by_core_1_epyc_power_2.csv
    done
done
