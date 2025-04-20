#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Test sieve performance with a specific block size
void test_block_size(int block_size, int m, int n) {
    int size = n - m + 1;
    bool* result = (bool*)malloc(size * sizeof(bool));
    memset(result, true, size * sizeof(bool));
    
    int root = sqrt(n) + 1;
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
    
    // Count primes
    int primeCount = 0;
    for (int i = 0; i < size; i++) {
        if (result[i] && (i + m) >= 2) {
            primeCount++;
        }
    }
    
    double performance = (double)size / timeTaken / 1000000;
    
    // Format output for easy parsing
    printf("BLOCK_SIZE=%d\n", block_size);
    printf("TIME=%.6f\n", timeTaken);
    printf("PERFORMANCE=%.3f\n", performance);
    printf("PRIMES=%d\n", primeCount);
    
    free(result);
    free(primeArray);
}

int main(int argc, char *argv[]) {
    int block_size = 49152;  // Default block size
    int m = 2;               // Default range start
    int n = 100000000;       // Default range end
    
    // Parse command line arguments if provided
    if (argc >= 2) {
        block_size = atoi(argv[1]);
    }
    if (argc >= 3) {
        m = atoi(argv[2]);
    }
    if (argc >= 4) {
        n = atoi(argv[3]);
    }
    
    // Test the specified block size
    test_block_size(block_size, m, n);
    
    return 0;
}