#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[])
{
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
    for (int i = 2; i*i*i*i <= n; i++) {
        if (primeArray[i] == true) {
            for (int j = i*i; j*j <= n; j+=i) {
                primeArray[j] = false;
            }
        }
    }

    // Mark non-primes in [m, n] using parallel processing with FS! optimization
    // Using schedule(dynamic) for load balancing as larger primes have less multiples
    #pragma omp parallel for schedule(dynamic)
    for (int i = 2; i <= root; i++) {
        if (primeArray[i]) {
            int firstMultiple = (m / i);
            if (firstMultiple <= 1) {
                firstMultiple = i + i;
            }
            else if (m % i) { 
                firstMultiple = (firstMultiple * i) + i;
            }
            else {
                firstMultiple = (firstMultiple * i);
            }
            
            for (int j = firstMultiple; j <= n; j+=i) {
                if (result[j-m]) result[j-m] = false; // FS! optimization - only modify if needed
            }
        }
    }
    
    clock_t endTime = clock();
    double timeTaken = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    // Count primes in range [m, n]
    int primeCount = 0;
    for (int i = 0; i < size; i++) {
        if (result[i] && (i + m) >= 2) {
            primeCount++;
        }
    }
    
    // Print results and timing information
    printf("Found %d primes in the range [%d, %d]\n", primeCount, m, n);
    printf("Time taken: %f seconds\n", timeTaken);
    printf("Performance: %.3f Mnums/sec\n", (double)size / timeTaken / 1000000);
    
    // Free allocated memory
    free(result);
    free(primeArray);
    
    return 0;
}