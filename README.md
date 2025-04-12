# PARALLEL-PROCESSING-UNI

## System Specification
- CPU: Intel Core i5-11400F
- 6 cores, 12 threads
- Base Frequency: 2.6 GHz, Turbo Boost up to 4.4 GHz

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
