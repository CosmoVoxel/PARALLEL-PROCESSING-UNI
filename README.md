# PARALLEL-PROCESSING-UNI

## System Specification
- CPU: Intel Core i5-11400F
- 6 cores, 12 threads
- Base Frequency: 2.6 GHz, Turbo Boost up to 4.4 GHz

## Performance Summary (Mnums/sec)
### Each result is an average of 10 runs

| Implementation | [2, 100000000] | [50000001, 100000000] | [2, 50000000] |
|----------------|-----------|--------------|-------------|
| prime1 | 113.611 | 109.195 | 113.265 |
| prime3 | 112.693 | 102.815 | 115.369 |
| prime3a | 179.039 | 169.155 | 176.468 |
| prime4 | 24.994 | 24.837 | 28.058 |
| prime4a | 33.732 | 29.334 | 30.991 |
| prime5 | 159.539 | 156.386 | 136.720 |
