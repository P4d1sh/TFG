#!/bin/bash

# CC=$(which python3)

# Define the directories/path to the files
HOME_DIR="/afs/atc.unican.es/u/j/juan"
TEST_DIR="${HOME_DIR}/PAPI-for-python-and-tf2/tests/mat-mul/C"
# SRC_DIR="${TEST_DIR}/src"
OUT_DIR="${TEST_DIR}/out"
# Creates the output folder
# mkdir -p ${OUT_DIR}

# Set the program to execute
PROGRAM="${TEST_DIR}/bin/main_papi"

# File where stdout and stderr content will be stored
STDERR_FILE="${OUT_DIR}/matmul.stderr"
TIME_FILE="${OUT_DIR}/time.txt"
# output_file="${OUT_DIR}/execution.txt"


# Calling to the Makefile to setup and compile the programs
make -C ${TEST_DIR} setup
make -C ${TEST_DIR} compile

# Now, we can define the params to pass to the program
declare -a MATRIX_TYPE=( "RAND" "SEQ" )
declare -a MATRIX_SIZE=( "128" "512" "2048" "8192" )
declare -a MULTIPLICATION_TYPE=( "NORMAL" "TRANSPOSE" "MULTITHREAD" )

# And set the number of executions to perform
NUM_EXECUTIONS=2

# We can start the measure
for mat_type in "${MATRIX_TYPE[@]}"; do
    for mat_size in "${MATRIX_SIZE[@]}"; do
        for mul_type in "${MULTIPLICATION_TYPE[@]}"; do
            # echo "\nStarting"
            printf '\nStarting measure of matrix:\t%s\t%s\t%s\n' "$mat_type" "$mat_size" "$mul_type"
            for (( i = 0; i < NUM_EXECUTIONS; i++ )); do
                { time \
                    eval taskset -c 2-31 "$PROGRAM" "${mat_type}" "${mat_size}" "${mul_type}" \
                    "${OUT_DIR}/matmul_${mat_type}_${mat_size}.csv" \
                    2>> "${STDERR_FILE}" ; } 2>> "${TIME_FILE}"
                # &>> "${output_file}"

            done
        done
    done
done
