#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

// The results are correct 
// Time measure : (2 max)  0.832803 s; (max/2 max)  0.377868 s (2 max/2) 0.416471 s
int main(int argc, char *argv[]) {
    int m = 2, n = 100000000; // Default values

    // Parse command line arguments if provided
    if (argc >= 2) {
        m = atoi(argv[1]);
    }
    if (argc >= 3) {
        n = atoi(argv[2]);
    }

    int rangeSize = n - m + 1;
    bool* result = (bool*)malloc(rangeSize * sizeof(bool));
    memset(result, true, rangeSize * sizeof(bool));

    int sqrtN = (int)sqrt(n) + 1;
    bool* primeArray = (bool*)malloc(sqrtN * sizeof(bool));
    memset(primeArray, true, sqrtN * sizeof(bool));
    clock_t startTime = clock();

    // Sieve of Eratosthenes to find primes up to sqrt(n)
    primeArray[0] = primeArray[1] = false;
    for (int i = 2; i * i <= sqrtN; i++) {
        if (primeArray[i]) {
            for (int j = i * i; j < sqrtN; j += i) {
                primeArray[j] = false;
                break;
            }
        }
    }

    // Mark non-primes in the range [m, n]
    for (int i = 2; i < sqrtN; i++) {
        if (primeArray[i]) {
            // Find the first multiple of i >= m
            int start = ((m + i - 1) / i) * i;
            if (start < i * 2) start = i * 2;
            for (int j = start; j <= n; j += i) {
                result[j - m] = false;
            }
        }
    }
    clock_t endTime = clock();
    double timeTaken = ((double)(endTime - startTime))/ CLOCKS_PER_SEC; 
    
    // Count primes in [m, n] instead of printing them
    int primeCount = 0;
    for (int i = 0; i < rangeSize; i++) {
        if (result[i] && (i + m) >= 2)
            primeCount++;
    }
    
    printf("Found %d primes in the range [%d, %d]\n", primeCount, m, n);
    printf("Time taken to find primes: %f seconds\n", timeTaken);
    printf("Performance: %.3f Mnums/sec\n", (double)rangeSize / timeTaken / 1000000);

    // Clean up
    free(result);
    free(primeArray);

    return 0;
}
