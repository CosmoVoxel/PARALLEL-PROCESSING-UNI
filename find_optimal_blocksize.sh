#!/bin/bash

# Script to determine the optimal block size for cache efficiency
# in prime number sieve algorithms

# Create build directory if it doesn't exist
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
BUILD_DIR="$DIR/build"
mkdir -p $BUILD_DIR

# Compile the test program
echo "Compiling block size test program..."
gcc -o $BUILD_DIR/test_block $DIR/test_block_size.c -lm

# Max value for prime calculation
MAX=100000000

# Number of runs for averaging
NUM_RUNS=3

# Initialize arrays
declare -A avg_times
declare -A avg_perf

# Test various block sizes based on cache hierarchy
BLOCK_SIZES=(
    # L1 cache-aligned sizes
    16384    # 16 KiB
    32768    # 32 KiB
    49152    # 48 KiB (approx size of L1 data cache)
    
    # L2 cache-aligned sizes
    65536    # 64 KiB
    98304    # 96 KiB
    131072   # 128 KiB
    262144   # 256 KiB
    524288   # 512 KiB (approx size of L2 cache)
    
    # Other potentially efficient sizes
    1048576  # 1 MiB
)

echo "Testing block sizes to find optimal value..."
echo "Each block size will be tested $NUM_RUNS times to get an average."
echo ""
echo "Block Size (bytes) | Avg Time (seconds) | Performance (Mnums/sec)"
echo "------------------ | ------------------ | ---------------------"

# Create output file for results
RESULT_FILE="$DIR/block_size_results.txt"
echo "# Block Size Testing Results" > $RESULT_FILE
echo "# Each result is an average of $NUM_RUNS runs" >> $RESULT_FILE
echo "# Format: block_size time_in_seconds performance_in_Mnums_per_sec" >> $RESULT_FILE
echo "" >> $RESULT_FILE

# Test each block size multiple times
for size in "${BLOCK_SIZES[@]}"; do
    total_time=0
    total_perf=0
    
    for ((run=1; run<=$NUM_RUNS; run++)); do
        output=$($BUILD_DIR/test_block $size 2 $MAX)
        
        # Extract time and performance from formatted output
        time_taken=$(echo "$output" | grep "TIME=" | cut -d= -f2)
        perf=$(echo "$output" | grep "PERFORMANCE=" | cut -d= -f2)
        
        total_time=$(echo "$total_time + $time_taken" | bc)
        total_perf=$(echo "$total_perf + $perf" | bc)
        
        echo "Testing block size $size - Run $run/$NUM_RUNS: $perf Mnums/sec ($time_taken s)"
    done
    
    # Calculate averages
    avg_time=$(echo "scale=6; $total_time / $NUM_RUNS" | bc)
    avg_performance=$(echo "scale=3; $total_perf / $NUM_RUNS" | bc)
    
    # Store results
    avg_times[$size]=$avg_time
    avg_perf[$size]=$avg_performance
    
    # Print results
    printf "%-18s | %-18s | %-21s\n" "$size" "$avg_time" "$avg_performance"
    
    # Save to file
    echo "$size $avg_time $avg_performance" >> $RESULT_FILE
done

# Find the best block size (highest performance)
best_size=""
best_perf=0

for size in "${BLOCK_SIZES[@]}"; do
    perf=${avg_perf[$size]}
    
    if (( $(echo "$perf > $best_perf" | bc -l) )); then
        best_perf=$perf
        best_size=$size
    fi
done

echo ""
echo "Optimal block size: $best_size bytes ($best_perf Mnums/sec)"

# Save the best block size to a file for easy access by other scripts
echo "$best_size" > "$DIR/optimal_block_size.txt"
echo "Results saved to $RESULT_FILE and optimal block size saved to optimal_block_size.txt"