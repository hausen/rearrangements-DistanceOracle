PROGS=query slice
OBJS=DistanceOracle.o NibbleArray.o Permutation.o
#CXX=icpc
CFLAGS=-O3

all: $(PROGS)

query: $(OBJS) Query.cc
	$(CXX) $(CFLAGS) $^ -o $@

slice: $(OBJS) Slice.cc
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	rm -f $(PROGS) $(OBJS)

%.o: %.cc %.h
	$(CXX) $(CFLAGS) $(DEFINES) -c $< -o $@

