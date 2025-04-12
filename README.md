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
| prime1 | 137.848 | 147.578 | 153.679 |
| prime3 | 129.236 | 139.367 | 144.238 |
| prime3a | 157.274 | 156.883 | 159.096 |
| prime4 | 74.180 | 81.003 | 84.311 |
| prime4a | 67.239 | 70.757 | 73.669 |
| prime5 | 131.724 | 128.875 | 134.565 |

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
