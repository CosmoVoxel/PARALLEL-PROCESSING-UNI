#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
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
    for (int i = 2; i * i <= root; i++) {
        if (primeArray[i] == true) {
            for (int j = i * i; j <= root; j += i) {
                primeArray[j] = false;
            }
        }
    }

    // Process in blocks to improve cache locality
    for (int blockStart = m; blockStart <= n; blockStart += block_size) {
        // Calculate the size of the current block
        int blockEnd = (blockStart + block_size - 1 < n) ? blockStart + block_size - 1 : n;
        
        // Allocate and initialize the block's sieve result array
        bool* block = (bool*)malloc((blockEnd - blockStart + 1) * sizeof(bool));
        memset(block, true, (blockEnd - blockStart + 1) * sizeof(bool));

        // Mark non-primes in the current block
        for (int i = 2; i <= root; i++) {
            if (primeArray[i]) {
                // Calculate the first multiple of i within the block
                int firstMultiple = (blockStart / i) * i;
                if (firstMultiple < blockStart) {
                    firstMultiple += i;
                }

                // Ensure the first multiple is not smaller than i*i
                if (firstMultiple == i) {
                    firstMultiple = i * i;
                }

                // Mark multiples of i in the current block
                for (int j = firstMultiple; j <= blockEnd; j += i) {
                    block[j - blockStart] = false;
                }
            }
        }

        // Copy block results into the main result array
        for (int i = blockStart; i <= blockEnd; i++) {
            result[i - m] = block[i - blockStart];
        }

        // Free block memory after processing
        free(block);
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
    
    printf("Found %d primes in the range [%d, %d] with block size %d\n", primeCount, m, n, block_size);
    printf("Time taken to find primes: %f seconds\n", timeTaken);
    printf("Performance: %.3f Mnums/sec\n", (double)size / timeTaken / 1000000);

    // Free allocated memory
    free(result);
    free(primeArray);
}