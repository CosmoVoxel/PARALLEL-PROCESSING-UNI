# PARALLEL-PROCESSING-UNI

## System Specification
- CPU: Intel Core i5-11400F
- 6 cores, 12 threads
- Base Frequency: 2.6 GHz, Turbo Boost up to 4.4 GHz

## Cache-Optimized Block Size
The optimal block size for this CPU was determined to be **262144 bytes**.
This was calculated by running performance tests across various block sizes aligned with the CPU's cache hierarchy.

## Performance Summary (Mnums/sec)
### Each result is an average of 10 runs

| Implementation | [2, 100000000] | [50000001, 100000000] | [2, 50000000] |
|----------------|-----------|--------------|-------------|
| prime1 | 137.292 | 146.134 | 153.799 |
| prime3 | 129.042 | 134.886 | 142.472 |
| prime3a | 157.340 | 157.716 | 160.272 |
| prime4 | 73.406 | 80.229 | 83.021 |
| prime4a | 67.021 | 70.198 | 72.310 |
| prime5 | 131.521 | 128.228 | 134.455 |

## Implementation Notes

### Block-Based Implementations
- **prime3a**: Sequential implementation with block-based approach for improved cache locality
- **prime5**: Parallel implementation with block-based approach using OpenMP

Both of these implementations use the optimal block size (262144 bytes) determined
by analyzing the CPU's cache hierarchy and running performance tests.

### Finding Optimal Block Size
Run the script `find_optimal_blocksize.sh` to determine the optimal block size for your CPU.
This script tests various block sizes and identifies which one provides the best performance.

```bash
./find_optimal_blocksize.sh
```

### Running Tests with the Optimal Block Size
The `test_ranges.sh` script automatically uses the optimal block size for block-based implementations.

```bash
./test_ranges.sh
```
## Test Environment
- Date: Sun Apr 13 02:40:03 UTC 2025
- CPU Model: AMD EPYC 7763 64-Core Processor
- CPU Cores: 4
- Memory: 15Gi
- OS: Ubuntu 24.04.2 LTS
- Kernel: 6.8.0-1021-azure
- Optimal block size: 262144 bytes

