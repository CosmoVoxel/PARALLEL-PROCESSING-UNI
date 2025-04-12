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
    
    // First sieve: mark non-primes up to sqrt(n)
    for (int i = 2; i * i <= root; i++) {
        if (primeArray[i]) {
            for (int j = i * i; j <= root; j += i) {
                primeArray[j] = false;
            }
        }
    }
    
    clock_t startTime = clock();
    
    // Process in blocks of the specified size
    for (int blockStart = m; blockStart <= n; blockStart += block_size) {
        int blockEnd = (blockStart + block_size - 1 < n) ? blockStart + block_size - 1 : n;
        
        bool* block = (bool*)malloc((blockEnd - blockStart + 1) * sizeof(bool));
        memset(block, true, (blockEnd - blockStart + 1) * sizeof(bool));
        
        for (int i = 2; i <= root; i++) {
            if (primeArray[i]) {
                int firstMultiple = (blockStart / i) * i;
                if (firstMultiple < blockStart) {
                    firstMultiple += i;
                }
                if (firstMultiple == i) {
                    firstMultiple = i * i;
                }
                
                for (int j = firstMultiple; j <= blockEnd; j += i) {
                    block[j - blockStart] = false;
                }
            }
        }
        
        for (int i = blockStart; i <= blockEnd; i++) {
            result[i - m] = block[i - blockStart];
        }
        
        free(block);
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