CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude

matrix: main.cpp src/Matrix.cpp src/Vector.cpp include/Matrix.hpp include/Vector.hpp
	$(CXX) $(CXXFLAGS) main.cpp src/Matrix.cpp src/Vector.cpp -o matrix

run: matrix
	./matrix

clean:
	rm -f matrix

.PHONY: run clean