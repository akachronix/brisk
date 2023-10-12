CC=g++
CXXFLAGS=-O2 -std=c++20 -Wall -pedantic 
BINDIR=bin
INCLUDEDIR=include
SRCDIR=src

.PHONY: clean
all: vector_benchmark

vector_benchmark: clean src/vector_benchmark.cpp
	mkdir $(BINDIR)
	$(CC) -I$(INCLUDEDIR) $(CXXFLAGS) $(word 2, $^) -o $(BINDIR)/$@ $(CXXLDFLAGS)

clean:
	rm -rf $(BINDIR) $(OBJDIR) *.log build/
