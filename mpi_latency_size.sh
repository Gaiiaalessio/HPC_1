#!/bin/bash
#SBATCH --job-name=HPC
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=24
#SBATCH --time=02:00:00
#SBATCH --partition THIN

module load openMPI/4.1.5/gnu/12.2.1

echo "Processes,Size,Latency" > output_processes_size.csv

# Numero di ripetizioni per ottenere una media
repetitions=10000

# Ciclo esterno per il numero di processori
for processes in {2..48..2}
do
    # Ciclo interno per la dimensione del messaggio
    for size in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384
    do
        # Esegui osu_bcast con numero di processi, dimensione fissa e numero di ripetizioni su due nodi
        result_bcast=$(mpirun -np $processes --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_bcast_algorithm 1 osu_bcast -m $size -x $repetitions -i $repetitions | tail -n 1 | awk '{print $2}')

        # Scrivi i risultati nel file CSV
        echo "$processes,$size,$result_bcast" >> output_processes_size.csv
    done
done

echo "Test completato. Risultati salvati in output.csv"

