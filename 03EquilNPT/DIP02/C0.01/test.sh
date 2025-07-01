#!/bin/bash --login

#SBATCH -J 03EM_C001
#SBATCH -N 1
#SBATCH --ntasks=32
#SBATCH -t 02:00:00
#SBATCH -o Tg.%j.out
### End BSUB Options and begin shell commands

module load FFTW
module load GCC
module load LAMMPS

TT=(0.85)

for tt in ${TT[*]}
do
    cd T$tt

    srun --exclusive lmp -in in.NPT -v TT $tt -v data_file ../../../../02CompressNetwork/DIP02/C0.01/compressed.dat &

    cd ../
done

wait