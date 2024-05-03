CXX_FLAGS=-std=c++17 -fopenmp -O2
EXECUTABLE=main
TEST_EXECUTABLE=test
BUILD_DIR=build
TESTING_DIR=testing

.PHONY: all marafet run clean testing test

all: directories testing $(BUILD_DIR)/main.o
	g++ $(CXX_FLAGS) $(BUILD_DIR)/main.o -o $(BUILD_DIR)/$(EXECUTABLE)

directories:
	mkdir -p $(BUILD_DIR)

testing: $(BUILD_DIR)/test.o
	g++ $(CXX_FLAGS) $< -o $(BUILD_DIR)/$(TEST_EXECUTABLE)

$(BUILD_DIR)/test.o: $(TESTING_DIR)/main.cpp
	g++ $(CXX_FLAGS) $< -c -o $@

test: $(BUILD_DIR)/$(TEST_EXECUTABLE)
	./$(BUILD_DIR)/$(TEST_EXECUTABLE)

marafet:
	find -name "*.cpp" -o -name "*.h" | xargs clang-format -i

$(BUILD_DIR)/main.o: main.cpp
	g++ $(CXX_FLAGS) $< -c -o $@

run:
	./$(BUILD_DIR)/$(EXECUTABLE)

clean:
	rm -r $(BUILD_DIR)
