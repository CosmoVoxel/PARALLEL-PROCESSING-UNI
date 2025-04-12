#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// The results are correct 
// Time measure : (2 max)  0.832697 s; (max/2 max)  0.379347 s (2 max/2)  0.372242 s
int main(int argc, char *argv[]) {
    int m = 2;
    int n = 100000000; // Default values

    // Parse command line arguments if provided
    if (argc >= 2) {
        m = atoi(argv[1]);
    }
    if (argc >= 3) {
        n = atoi(argv[2]);
    }

    int size = n - m + 1;

    // Allocate and initialize result array
    bool* result = (bool*)malloc(size * sizeof(bool));
    memset(result, true, size * sizeof(bool));

    int root = sqrt(n) + 1;

    // Allocate and initialize primeArray
    bool* primeArray = (bool*)malloc((root + 1) * sizeof(bool));
    memset(primeArray, true, (root + 1) * sizeof(bool));
    clock_t startTime = clock();
    // First sieve: mark non-primes up to sqrt(n)
    for (int i = 2; i * i * i * i <= n; i++) {
        if (primeArray[i] == true) {
            for (int j = i * i; j * j <= n; j += i) {
                primeArray[j] = false;
            }
        }
    }

    // Mark non-primes in [m, n]
    for (int i = 2; i * i <= n; i++) {
        if (primeArray[i]) {
            int firstMultiple = m / i;
            if (firstMultiple <= 1) {
                firstMultiple = i + i;
            } else if (m % i) {
                firstMultiple = (firstMultiple * i) + i;
            } else {
                firstMultiple = (firstMultiple * i);
            }

            for (int j = firstMultiple; j <= n; j += i) {
                result[j - m] = false;
            }
        }
    }
     clock_t endTime = clock();
    double timeTaken = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    // Count primes instead of printing them
    int primeCount = 0;
    for (int i = 0; i < size; i++) {
        if (result[i] && (i + m) >= 2) {
            primeCount++;
        }
    }
    
    printf("Found %d primes in the range [%d, %d]\n", primeCount, m, n);
    printf("Time taken to find primes: %f seconds\n", timeTaken);
    printf("Performance: %.3f Mnums/sec\n", (double)size / timeTaken / 1000000);
    
    // Free allocated memory
    free(result);
    free(primeArray);

    return 0;
}
