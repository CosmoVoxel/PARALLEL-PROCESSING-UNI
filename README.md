# PARALLEL-PROCESSING-UNI

## System Specification
- CPU: Intel Core i5-11400F
- 6 cores, 12 threads
- Base Frequency: 2.6 GHz, Turbo Boost up to 4.4 GHz

### Results

| Implementation | Execution Time (seconds) |
|----------------|-------------------------|
| prime1         | 0.453474               |
| prime3         | 0.480881               |
| prime3a        | 0.286371               |
| prime4         | 1.6 (dynamic)          |
| prime4a        | 1.43 (dynamic)         |
| prime5         | 0.25 (dynamic)         |

### Command to run

```bash
(echo "=== prime1 ===" && ./prime1 && echo -e "\n=== prime3 ===" && ./prime3 && echo -e "\n=== prime3a ===" && ./prime3a && echo -e "\n=== prime4 ===" && ./prime4 && echo -e "\n=== prime4a ===" && ./prime4a && echo -e "\n=== prime5 ===" && ./prime5) > results.txt && cat results.txt
```