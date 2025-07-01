#!/bin/bash --login

#SBATCH -J Tg_for_Melt_NN40
#SBATCH -N 1
#SBATCH --ntasks=32
#SBATCH -t 010:00:00
#SBATCH -o Tg.%j.out
### End BSUB Options and begin shell commands

module load FFTW
module load GCC
module load LAMMPS

TT=(0.45 0.65 0.85 1.05 1.20) #array of temperatures you'll run the simulation for

for tt in ${TT[*]}
do
    cd T$tt

    srun --exclusive lmp -in in.NPT -v TT $tt -v data_file ../../../../04HighTempEquiMelt/NN040/equi.dat &

    cd ../
done

wait