#!/bin/bash --login
#SBATCH -J 02CN02
#SBATCH -N 2
#SBATCH --ntasks=64
#SBATCH -t 1:00:00

module load LAMMPS

srun lmp < in.compress -v data_file /mnt/research/Cheng_Polymer/CCF_PROJ/01BuildNetwork/DIP02/C0.75/polymer.dat -v TT 1.0 -v rho 0.85