CXXFLAGS := $(shell llvm-config --cxxflags --ldflags --system-libs --libs core orcjit native)

khcc: main.o
	$(CXX) -o khcc main.o $(CXXFLAGS)

test: khcc
	./test.sh

clean:
	rm -f khcc *.o *~ tmp* a.out

.PHONY: test clean