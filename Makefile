CXX_FLAGS=-std=c++17 -fopenmp -O2
EXECUTABLE=main
BUILD_DIR=build

all: marafet main.o
	g++ $(CXX_FLAGS) $(BUILD_DIR)/main.o -o $(BUILD_DIR)/$(EXECUTABLE)

marafet:
	clang-format -i *.h *.cpp

main.o: main.cpp
	g++ $(CXX_FLAGS) $< -c -o $(BUILD_DIR)/$@

run:
	./$(BUILD_DIR)/$(EXECUTABLE)

clean:
	rm $(BUILD_DIR)/main*

