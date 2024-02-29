#!/bin/bash
#SBATCH --job-name=test-re
#SBATCH --time=00:30:00
#SBATCH --account=project_462000008
#SBATCH --partition=standard-g
##SBATCH --partition=ju-strategic-g
###SBATCH --partition=dev-g
#SBATCH --nodes=128
#SBATCH --cpus-per-task=1
#SBATCH --gpus-per-node=8
#SBATCH --ntasks-per-node=8
###SBATCH --nodelist=nid0061[52-59]
#SBATCH -e err_%j.log
#SBATCH -o out_%j.log
###SBATCH --nodelist=nid006153
#SBATCH --exclude=nid[006696-006706,006757-006764,006956-006965,007119-007128,007510-007588,007791-007800]
srun -l ./g.sh
