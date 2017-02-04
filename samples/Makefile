CXX=g++
CXXFLAGS+=-I../include
LDFLAGS:=-L../build
LIBS:=-lvideohelper -lpthread

SRCS:= memory_test.cpp
TARGETS:=test_memory

all: $(TARGETS)
test_memory: memory_test.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f *.o test_* *.log
