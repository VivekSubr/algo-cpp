cmake: clean
	mkdir -p build && cd build && cmake ..

clean:
	rm -fr build

test:
	./build/GraphTest

include $(shell git rev-parse --show-toplevel)/visualizer/visualize.mk