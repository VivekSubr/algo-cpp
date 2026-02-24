cmake: clean
	mkdir -p build && cd build && cmake  -DCMAKE_BUILD_TYPE=Debug ..

clean:
	rm -fr build

test:
	./build/bloom_filter_test

debug:
	gdb -d ${PWD} ${PWD}/build/bloom_filter_test