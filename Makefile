CXX_FLAGS=-std=c++17 -fopenmp -O2
EXECUTABLE=main
BUILD_DIR=build

all: main.o
	g++ $(CXX_FLAGS) $(BUILD_DIR)/$< -o $(BUILD_DIR)/$(EXECUTABLE)

main.o: main.cpp
	g++ $(CXX_FLAGS) $< -c -o $(BUILD_DIR)/$@

run:
	./$(BUILD_DIR)/$(EXECUTABLE)

clean:
	rm $(BUILD_DIR)/main*

