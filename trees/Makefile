cmake: clean
	mkdir -p build && cd build && cmake ..

clean:
	rm -fr build

test:
	./build/treeAlgo

include $(shell git rev-parse --show-toplevel)/visualizer/visualize.mk