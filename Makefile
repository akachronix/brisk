CC=g++
CXXFLAGS=-O2 -std=c++17 -Wall -pedantic 
CXXLDFLAGS=
CXXSTD=

BINDIR=bin
INCLUDEDIR=include
LIBDIR=lib
OBJDIR=obj
SRCDIR=src

TARGET=brisk

.PHONY: clean

all: $(TARGET)

$(TARGET): clean src/test.cpp
	mkdir $(BINDIR)
	$(CC) -I$(INCLUDEDIR) -L$(LIBDIR) $(CXXFLAGS) $(word 2, $^) -o $(BINDIR)/$(TARGET) $(CXXLDFLAGS)

clean:
	rm -rf $(BINDIR) $(OBJDIR) *.log build/
