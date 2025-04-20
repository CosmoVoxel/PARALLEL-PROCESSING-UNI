# PARALLEL-PROCESSING-UNI

## Performance Summary (Mnums/sec)
### Each result is an average of 10 runs



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
- Date: Sun Apr 20 01:17:58 UTC 2025
- CPU Model: AMD EPYC 7763 64-Core Processor
- CPU Cores: 4
- Memory: 15Gi
- OS: Ubuntu 24.04.2 LTS
- Kernel: 6.8.0-1021-azure
- Optimal block size: 262144 bytes

