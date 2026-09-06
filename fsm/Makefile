cmake: clean
	mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=DEBUG ..

clean:
	rm -fr build

test:
	./build/FSMexampleTest

include $(shell git rev-parse --show-toplevel)/visualizer/visualize.mk