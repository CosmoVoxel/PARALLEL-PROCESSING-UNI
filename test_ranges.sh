#!/bin/bash

# Define the maximum value
MAX=100000000
HALF_MAX=$((MAX / 2))

# Number of runs for averaging
NUM_RUNS=10

# Current directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Create build directory if it doesn't exist
BUILD_DIR="$DIR/build"
mkdir -p $BUILD_DIR

# Check if optimal block size has been determined
if [ ! -f "$DIR/optimal_block_size.txt" ]; then
    echo "Finding optimal block size first..."
    bash "$DIR/find_optimal_blocksize.sh"
fi

# Read optimal block size
OPTIMAL_BLOCK_SIZE=$(cat "$DIR/optimal_block_size.txt")
echo "Using optimal block size: $OPTIMAL_BLOCK_SIZE"

# Compile all programs with OpenMP support
echo "Compiling programs..."
gcc -o $BUILD_DIR/prime1 $DIR/prime1.c -lm
gcc -o $BUILD_DIR/prime3 $DIR/prime3.c -lm
gcc -o $BUILD_DIR/prime3a $DIR/prime3a.c -lm
gcc -o $BUILD_DIR/prime4 $DIR/prime4.c -lm -fopenmp
gcc -o $BUILD_DIR/prime4a $DIR/prime4a.c -lm -fopenmp
gcc -o $BUILD_DIR/prime5 $DIR/prime5.c -lm -fopenmp

# Create results file
OUTPUT_FILE="$DIR/range_results.txt"
echo "# Performance Results for Different Ranges" > $OUTPUT_FILE
echo "MAX = $MAX" >> $OUTPUT_FILE
echo "# Each result is an average of $NUM_RUNS runs" >> $OUTPUT_FILE
echo "# Optimal block size: $OPTIMAL_BLOCK_SIZE" >> $OUTPUT_FILE
echo "" >> $OUTPUT_FILE

# Initialize arrays to store performance metrics
declare -A perf_metrics

echo "Running tests for ranges..."

# Function to run a program multiple times and return the average performance
run_multiple_times() {
    local program=$1
    local start=$2
    local end=$3
    local block_size=$4
    local use_block_size=$5
    
    total_perf=0
    total_time=0
    
    for ((run=1; run<=$NUM_RUNS; run++)); do
        # Run with or without block size parameter
        if [ "$use_block_size" = true ]; then
            output=$($program $start $end $block_size)
        else
            output=$($program $start $end)
        fi
        
        # Extract time and performance
        time_taken=$(echo "$output" | grep -E "Time taken|Time:" | awk '{print $(NF-1)}')
        perf=$(echo "$output" | grep "Performance:" | awk '{print $2}')
        
        total_perf=$(echo "$total_perf + $perf" | bc)
        total_time=$(echo "$total_time + $time_taken" | bc)
        
        # Show progress
        echo -ne "   Run $run/$NUM_RUNS: $perf Mnums/sec\r"
    done
    
    # Calculate averages
    avg_perf=$(echo "scale=3; $total_perf / $NUM_RUNS" | bc)
    avg_time=$(echo "scale=6; $total_time / $NUM_RUNS" | bc)
    
    # Extract prime count from last run (should be same for all runs)
    prime_count=$(echo "$output" | grep "Found" | awk '{print $2}')
    
    echo -e "\n   Average of $NUM_RUNS runs: $avg_perf Mnums/sec ($avg_time seconds)"
    
    # Return the results as a string
    echo "Found $prime_count primes in the range [$start, $end]"
    echo "Time taken: $avg_time seconds (average of $NUM_RUNS runs)"
    echo "Performance: $avg_perf Mnums/sec (average of $NUM_RUNS runs)"
}

# Run tests for all ranges
{
    echo "## Range [2, $MAX]"
    echo ""
    
    for prog in prime1 prime3 prime4 prime4a; do
        echo "### $prog"
        echo "Running $NUM_RUNS times for range [2, $MAX]..."
        output=$(run_multiple_times $BUILD_DIR/$prog 2 $MAX "" false)
        echo "$output" >> $OUTPUT_FILE
        echo "" >> $OUTPUT_FILE
        
        # Extract and store performance metric
        perf=$(echo "$output" | grep "Performance:" | awk '{print $2}')
        perf_metrics["${prog}_range1"]=$perf
    done
    
    # Run block-based programs with optimal block size
    for prog in prime3a prime5; do
        echo "### $prog"
        echo "Running $NUM_RUNS times for range [2, $MAX] with block size $OPTIMAL_BLOCK_SIZE..."
        output=$(run_multiple_times $BUILD_DIR/$prog 2 $MAX $OPTIMAL_BLOCK_SIZE true)
        echo "$output" >> $OUTPUT_FILE
        echo "" >> $OUTPUT_FILE
        
        # Extract and store performance metric
        perf=$(echo "$output" | grep "Performance:" | awk '{print $2}')
        perf_metrics["${prog}_range1"]=$perf
    done
    
    echo "## Range [$(($HALF_MAX+1)), $MAX]"
    echo ""
    
    for prog in prime1 prime3 prime4 prime4a; do
        echo "### $prog"
        echo "Running $NUM_RUNS times for range [$(($HALF_MAX+1)), $MAX]..."
        output=$(run_multiple_times $BUILD_DIR/$prog $(($HALF_MAX+1)) $MAX "" false)
        echo "$output" >> $OUTPUT_FILE
        echo "" >> $OUTPUT_FILE
        
        # Extract and store performance metric
        perf=$(echo "$output" | grep "Performance:" | awk '{print $2}')
        perf_metrics["${prog}_range2"]=$perf
    done
    
    # Run block-based programs with optimal block size
    for prog in prime3a prime5; do
        echo "### $prog"
        echo "Running $NUM_RUNS times for range [$(($HALF_MAX+1)), $MAX] with block size $OPTIMAL_BLOCK_SIZE..."
        output=$(run_multiple_times $BUILD_DIR/$prog $(($HALF_MAX+1)) $MAX $OPTIMAL_BLOCK_SIZE true)
        echo "$output" >> $OUTPUT_FILE
        echo "" >> $OUTPUT_FILE
        
        # Extract and store performance metric
        perf=$(echo "$output" | grep "Performance:" | awk '{print $2}')
        perf_metrics["${prog}_range2"]=$perf
    done
    
    echo "## Range [2, $HALF_MAX]"
    echo ""
    
    for prog in prime1 prime3 prime4 prime4a; do
        echo "### $prog"
        echo "Running $NUM_RUNS times for range [2, $HALF_MAX]..."
        output=$(run_multiple_times $BUILD_DIR/$prog 2 $HALF_MAX "" false)
        echo "$output" >> $OUTPUT_FILE
        echo "" >> $OUTPUT_FILE
        
        # Extract and store performance metric
        perf=$(echo "$output" | grep "Performance:" | awk '{print $2}')
        perf_metrics["${prog}_range3"]=$perf
    done
    
    # Run block-based programs with optimal block size
    for prog in prime3a prime5; do
        echo "### $prog"
        echo "Running $NUM_RUNS times for range [2, $HALF_MAX] with block size $OPTIMAL_BLOCK_SIZE..."
        output=$(run_multiple_times $BUILD_DIR/$prog 2 $HALF_MAX $OPTIMAL_BLOCK_SIZE true)
        echo "$output" >> $OUTPUT_FILE
        echo "" >> $OUTPUT_FILE
        
        # Extract and store performance metric
        perf=$(echo "$output" | grep "Performance:" | awk '{print $2}')
        perf_metrics["${prog}_range3"]=$perf
    done
} >> $OUTPUT_FILE

echo "Tests completed. Results saved in $OUTPUT_FILE"

# Generate a summary table for README
echo "Generating summary table for README..."
{
    echo "# PARALLEL-PROCESSING-UNI"
    echo ""
    echo "## System Specification"
    echo "- CPU: Intel Core i5-11400F"
    echo "- 6 cores, 12 threads"
    echo "- Base Frequency: 2.6 GHz, Turbo Boost up to 4.4 GHz"
    echo ""
    echo "## Cache-Optimized Block Size"
    echo "The optimal block size for this CPU was determined to be **$OPTIMAL_BLOCK_SIZE bytes**."
    echo "This was calculated by running performance tests across various block sizes aligned with the CPU's cache hierarchy."
    echo ""
    echo "## Performance Summary (Mnums/sec)"
    echo "### Each result is an average of $NUM_RUNS runs"
    echo ""
    echo "| Implementation | [2, $MAX] | [$((HALF_MAX+1)), $MAX] | [2, $HALF_MAX] |"
    echo "|----------------|-----------|--------------|-------------|"
    
    for prog in prime1 prime3 prime3a prime4 prime4a prime5; do
        r1="${perf_metrics[${prog}_range1]}"
        r2="${perf_metrics[${prog}_range2]}"
        r3="${perf_metrics[${prog}_range3]}"
        
        echo "| $prog | $r1 | $r2 | $r3 |"
    done
    
    echo ""
    echo "## Implementation Notes"
    echo ""
    echo "### Block-Based Implementations"
    echo "- **prime3a**: Sequential implementation with block-based approach for improved cache locality"
    echo "- **prime5**: Parallel implementation with block-based approach using OpenMP"
    echo ""
    echo "Both of these implementations use the optimal block size ($OPTIMAL_BLOCK_SIZE bytes) determined"
    echo "by analyzing the CPU's cache hierarchy and running performance tests."
    echo ""
    echo "### Finding Optimal Block Size"
    echo "Run the script \`find_optimal_blocksize.sh\` to determine the optimal block size for your CPU."
    echo "This script tests various block sizes and identifies which one provides the best performance."
    echo ""
    echo '```bash'
    echo "./find_optimal_blocksize.sh"
    echo '```'
    echo ""
    echo "### Running Tests with the Optimal Block Size"
    echo "The \`test_ranges.sh\` script automatically uses the optimal block size for block-based implementations."
    echo ""
    echo '```bash'
    echo "./test_ranges.sh"
    echo '```'
    
} > "$DIR/README.md"

echo "README.md has been updated with average performance results"