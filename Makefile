# Configuration
CC = gcc
CFLAGS = -lm -Wall -O3 
OPENMP_FLAGS = -fopenmp
BUILD_DIR = build

# Ensure build directory exists
$(shell mkdir -p $(BUILD_DIR))

# Default target
all: clean build run

# Build all executables
build: prime1 prime2 prime3 prime3a prime4 prime4a prime5 test_block

# Individual build targets
prime1: prime1.c
	$(CC) $< -o $(BUILD_DIR)/$@ $(CFLAGS)

prime2: prime2.c
	$(CC) $< -o $(BUILD_DIR)/$@ $(CFLAGS)

prime3: prime3.c
	$(CC) $< -o $(BUILD_DIR)/$@ $(CFLAGS)

prime3a: prime3a.c
	$(CC) $< -o $(BUILD_DIR)/$@ $(CFLAGS)

prime4: prime4.c
	$(CC) $(OPENMP_FLAGS) $< -o $(BUILD_DIR)/$@ $(CFLAGS)

prime4a: prime4a.c
	$(CC) $(OPENMP_FLAGS) $< -o $(BUILD_DIR)/$@ $(CFLAGS)

prime5: prime5.c
	$(CC) $(OPENMP_FLAGS) $< -o $(BUILD_DIR)/$@ $(CFLAGS)

test_block: test_block_size.c
	$(CC) $< -o $(BUILD_DIR)/$@ $(CFLAGS)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)/*
	@mkdir -p $(BUILD_DIR)

run:
	@echo "Running tests..."
	@$(BUILD_DIR)/prime1 
	@$(BUILD_DIR)/prime2 
	@$(BUILD_DIR)/prime3 
	@$(BUILD_DIR)/prime3a 
	@$(BUILD_DIR)/prime4 
	@$(BUILD_DIR)/prime4a 
	@$(BUILD_DIR)/prime5 

# Show help
help:
	@echo "Prime Number Sieve Performance Testing"
	@echo ""
	@echo "Available targets:"
	@echo "  all       - Clean, build all, optimize block size, run tests, generate report"
	@echo "  build     - Build all executables"
	@echo "  clean     - Clean build artifacts"
	@echo "  run       - Run all tests"
	@echo ""
	@echo "Individual build targets:"
	@echo "  prime1 prime2 prime3 prime3a prime4 prime4a prime5 test_block"