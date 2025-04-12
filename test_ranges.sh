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
echo "" >> $OUTPUT_FILE

# Initialize arrays to store performance metrics
declare -A perf_metrics

echo "Running tests for ranges..."

# Function to run a program multiple times and return the average performance
run_multiple_times() {
    local program=$1
    local start=$2
    local end=$3
    
    total_perf=0
    total_time=0
    
    for ((run=1; run<=$NUM_RUNS; run++)); do
        output=$($program $start $end)
        
        # Extract time and performance
        time_taken=$(echo "$output" | grep -E "Time taken" | awk '{print $(NF-1)}')
        perf=$(echo "$output" | grep "Performance" | awk '{print $2}')
        
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
    
    for prog in prime1 prime3 prime3a prime4 prime4a prime5; do
        echo "### $prog"
        echo "Running $NUM_RUNS times for range [2, $MAX]..."
        output=$(run_multiple_times $BUILD_DIR/$prog 2 $MAX)
        echo "$output" >> $OUTPUT_FILE
        echo "" >> $OUTPUT_FILE
        
        # Extract and store performance metric
        perf=$(echo "$output" | grep "Performance" | awk '{print $2}')
        perf_metrics["${prog}_range1"]=$perf
    done
    
    echo "## Range [$(($HALF_MAX+1)), $MAX]"
    echo ""
    
    for prog in prime1 prime3 prime3a prime4 prime4a prime5; do
        echo "### $prog"
        echo "Running $NUM_RUNS times for range [$(($HALF_MAX+1)), $MAX]..."
        output=$(run_multiple_times $BUILD_DIR/$prog $(($HALF_MAX+1)) $MAX)
        echo "$output" >> $OUTPUT_FILE
        echo "" >> $OUTPUT_FILE
        
        # Extract and store performance metric
        perf=$(echo "$output" | grep "Performance" | awk '{print $2}')
        perf_metrics["${prog}_range2"]=$perf
    done
    
    echo "## Range [2, $HALF_MAX]"
    echo ""
    
    for prog in prime1 prime3 prime3a prime4 prime4a prime5; do
        echo "### $prog"
        echo "Running $NUM_RUNS times for range [2, $HALF_MAX]..."
        output=$(run_multiple_times $BUILD_DIR/$prog 2 $HALF_MAX)
        echo "$output" >> $OUTPUT_FILE
        echo "" >> $OUTPUT_FILE
        
        # Extract and store performance metric
        perf=$(echo "$output" | grep "Performance" | awk '{print $2}')
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
} > "$DIR/README.md"

echo "README.md has been updated with average performance results"