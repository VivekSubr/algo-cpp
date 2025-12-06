cmake: clean
	mkdir -p build && cd build && cmake  -DCMAKE_BUILD_TYPE=Debug ..

clean:
	rm -fr build

test:
	./build/2dTest

debug:
	gdb -d ${PWD} ${PWD}/build/2dTest