#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
    int m = 2;
    int n = 50000000;

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
    // Based on analysis from blockLen file, a blockSize of 49152 seems optimal
    int blockSize = 49152;
    int numberOfBlocks = (n - m) / blockSize;
    if ((n - m) % blockSize != 0) {
        numberOfBlocks++;
    }
    
    // Mark non-primes in blocks using parallel processing
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < numberOfBlocks; i++) {
        int low = m + i * blockSize;
        int high = m + i * blockSize + blockSize;
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
    printf("Found %d primes in the range [%d, %d]\n", primeCount, m, n);
    printf("Time taken: %f seconds\n", timeTaken);
    
    // Free allocated memory
    free(result);
    free(primeArray);
    
    return 0;
}