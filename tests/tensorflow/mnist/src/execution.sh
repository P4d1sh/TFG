#!/bin/bash

# fixed variables
CC=$(which python3)
SRC_DIR="/afs/atc.unican.es/u/j/juan/models/official/vision/image_classification"
program="mnist_main.py"

# Creates the output folder
OUT_DIR="/afs/atc.unican.es/u/j/juan/mnist_out"
# mkdir -p ${OUT_DIR}

declare -a inter_list=( 0 1 2 ) #4 8 16 32 )
declare -a intra_list=( 0 1 2 ) #4 8 16 32 )

# declare -a n_threads_list=( "2" "2-3" "2-5" "2-9" "2-17" "2-31" ) # nthreads = 1, 2, 4, 8, 16, 30
declare -a n_threads_list=( "2" "2-3" "2-31" ) # nthreads = 1, 2, 30

num_executions=5

for thread in "${n_threads_list[@]}"; do
    for inter in "${inter_list[@]}"; do
        for intra in "${intra_list[@]}"; do
            for (( i = 0; i < num_executions; i++ )); do

                echo eval taskset -c "${thread}" "${CC}" "${SRC_DIR}/${program}" "${inter}" "${intra}" "${OUT_DIR}/taskset-${thread}_inter-${inter}_intra-${intra}.csv" \
                    --model_dir="$MODEL_DIR" --data_dir="$DATA_DIR" --train_epochs=10 --distribution_strategy=one_device --download

            done
        done
    done
done
