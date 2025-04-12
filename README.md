# PARALLEL-PROCESSING-UNI

## System Specification
- CPU: Intel Core i5-11400F
- 6 cores, 12 threads
- Base Frequency: 2.6 GHz, Turbo Boost up to 4.4 GHz

## Cache-Optimized Block Size
The optimal block size for this CPU was determined to be **524288 bytes**.
This was calculated by running performance tests across various block sizes aligned with the CPU's cache hierarchy.

## Performance Summary (Mnums/sec)
### Each result is an average of 10 runs

| Implementation | [2, 100000000] | [50000001, 100000000] | [2, 50000000] |
|----------------|-----------|--------------|-------------|
| prime1 | 107.189 | 99.148 | 100.515 |
| prime3 | 99.283 | 94.553 | 108.149 |
| prime3a | 169.668 | 166.418 | 180.724 |
| prime4 | 19.042 | 22.109 | 27.064 |
| prime4a | 24.541 | 30.554 | 34.589 |
| prime5 | 130.148 | 128.736 | 152.378 |

## Implementation Notes

### Block-Based Implementations
- **prime3a**: Sequential implementation with block-based approach for improved cache locality
- **prime5**: Parallel implementation with block-based approach using OpenMP

Both of these implementations use the optimal block size (524288 bytes) determined
by analyzing the CPU's cache hierarchy and running performance tests.

### Finding Optimal Block Size
Run the script `find_optimal_blocksize.sh` to determine the optimal block size for your CPU.
This script tests various block sizes and identifies which one provides the best performance.



### Running Tests with the Optimal Block Size
The `test_ranges.sh` script automatically uses the optimal block size for block-based implementations.


