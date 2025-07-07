#!/bin/bash --login

#SBATCH -J PE.007
#SBATCH -o PE.007.%j.out
#SBATCH -t 2:00:00
#SBATCH -N 5
#SBATCH --ntasks=32
#SBATCH --mem-per-cpu=4G

# Use CPU-only version of LAMMPS (update if needed)
module load LAMMPS
module load FFTW

TT=(0.85)
EE=0.0
CC=0.01
##CC=(0.01 0.03 0.1 0.3 0.75)

for tt in "${TT[@]}"
do
    cd T$tt

    datFile="prod.restart2.006"

    # CPU-only srun command
    srun --exclusive -N 5 -n 32 -c 1 \
        lmp -in in.dpd \
        -v EE $EE -v TT $TT -v datFile $datFile -v random $RANDOM -v idx 007

    cd ../
done
