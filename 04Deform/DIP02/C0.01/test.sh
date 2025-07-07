#!/bin/bash --login

#SBATCH -J 04DEF_0.01
#SBATCH -N 1
#SBATCH --ntasks=32
#SBATCH -t 02:00:00
#SBATCH -o test.%j.out
### End BSUB Options and begin shell commands

module load FFTW
module load GCC
module load LAMMPS

TT=(0.85)

for tt in ${TT[*]}
do
    cd T$tt

    srun --exclusive lmp -in in.deform -v TT $tt -v random $RANDOM -v data_file ../../../../03EquilNPT/DIP02/C0.01/T0.85/equi.dat &

    cd ../
done

wait