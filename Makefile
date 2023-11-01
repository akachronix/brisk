CC=g++
CXXFLAGS=-O2 -std=c++20 -Wall -pedantic 
BINDIR=bin
INCLUDEDIR=include
SRCDIR=src

.PHONY: clean
all: vector_benchmark string_refactor threads array_swap_test

array_swap_test: bin src/array_swap_test.cpp
	$(CC) -I$(INCLUDEDIR) $(CXXFLAGS) $(word 2, $^) -o $(BINDIR)/$@ $(CXXLDFLAGS)

threads: bin src/threads.cpp
	$(CC) -I$(INCLUDEDIR) $(CXXFLAGS) $(word 2, $^) -o $(BINDIR)/$@ $(CXXLDFLAGS)

vector_benchmark: bin src/vector_benchmark.cpp
	$(CC) -I$(INCLUDEDIR) $(CXXFLAGS) $(word 2, $^) -o $(BINDIR)/$@ $(CXXLDFLAGS)

string_refactor: bin src/string_refactor.cpp
	$(CC) -I$(INCLUDEDIR) $(CXXFLAGS) $(word 2, $^) -o $(BINDIR)/$@ $(CXXLDFLAGS)

bin:
	mkdir $@

clean:
	rm -rf $(BINDIR) $(OBJDIR) *.log build/
