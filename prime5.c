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
    int block_size = 262144; // Default block size
    
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

    // Block-based approach for better cache locality
    int numberOfBlocks = (n - m) / block_size;
    if ((n - m) % block_size != 0) {
        numberOfBlocks++;
    }
    
    // Mark non-primes in blocks using parallel processing
    #pragma omp parallel for schedule(runtime)
    for (int i = 0; i < numberOfBlocks; i++) {
        int low = m + i * block_size;
        int high = m + i * block_size + block_size;
        if (high > n) {
            high = n;
        }
        
        for (int j = 2; j * j <= high; j++) {
            if (primeArray[j]) {
                int firstMultiple = (low / j);
                if (firstMultiple <= 1) {
                    firstMultiple = j + j;
                }
                else if (low % j) {
                    firstMultiple = (firstMultiple * j) + j;
                }
                else {
                    firstMultiple = (firstMultiple * j);
                }
                
                for (int k = firstMultiple; k <= high; k += j) {
                    result[k - m] = false;
                }
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
    printf("Found %d primes in the range [%d, %d] with block size %d\n", primeCount, m, n, block_size);
    printf("Time taken: %f seconds\n", timeTaken);
    printf("Performance: %.3f Mnums/sec\n", (double)size / timeTaken / 1000000);
    
    // Free allocated memory
    free(result);
    free(primeArray);
    
    return 0;
}