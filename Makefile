
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC = src/hashmap.cpp src/heap.cpp src/queue.cpp src/graph.cpp src/tree.cpp src/ledger.cpp src/data.cpp src/display.cpp

all: waste_system

waste_system: $(SRC) src/main.cpp
	$(CXX) $(CXXFLAGS) $(SRC) src/main.cpp -o waste_system

tests: $(SRC) tests/tests.cpp
	$(CXX) $(CXXFLAGS) $(SRC) tests/tests.cpp -o tests_runner
	./tests_runner

run: waste_system
	mkdir -p data
	./waste_system

clean:
	rm -f waste_system tests_runner

.PHONY: all tests run clean
