#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

// The results are correct 
// Time measure : (2 max)  0.832803 s; (max/2 max)  0.377868 s (2 max/2) 0.416471 s
int main() {
    int m = 2, n = 50000000;

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
    // Print primes in [m, n]
    printf("Primes between %d and %d:\n", m, n);
    for (int i = 0; i < rangeSize; i++) {
        if (result[i] && (i + m) >= 2)
            printf("%d ", i + m);
    }
 printf("\nTime taken to find primes: %f seconds\n", timeTaken);

    // Clean up
    free(result);
    free(primeArray);

    return 0;
}
