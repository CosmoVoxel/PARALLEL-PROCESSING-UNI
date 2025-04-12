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
| prime1 | 111.025 | 113.360 | 118.887 |
| prime3 | 110.965 | 115.832 | 119.190 |
| prime3a | 173.510 | 173.506 | 177.920 |
| prime4 | 21.826 | 26.975 | 30.177 |
| prime4a | 29.058 | 34.071 | 32.354 |
| prime5 | 164.815 | 151.487 | 169.545 |

## Implementation Notes

### Block-Based Implementations
- **prime3a**: Sequential implementation with block-based approach for improved cache locality
- **prime5**: Parallel implementation with block-based approach using OpenMP

Both of these implementations use the optimal block size (524288 bytes) determined
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

## Automated Performance Metrics

This repository uses GitHub Actions to automatically update performance metrics when:

1. Changes are pushed to C files or shell scripts
2. A pull request is created or updated that affects code
3. Someone manually triggers the workflow
4. Once a week on Sunday (scheduled run)

The workflow performs the following actions:

1. Finds the optimal block size for the GitHub Actions runner's CPU
2. Runs all performance tests using this optimal block size
3. Updates the README.md with the latest performance metrics
4. Updates system information in the README

This ensures that performance metrics are always up-to-date and reflect the current state of the codebase.

To manually trigger the workflow:
1. Go to the "Actions" tab in the GitHub repository
2. Select "Update Performance Metrics" workflow 
3. Click "Run workflow"


