cmake: clean
	mkdir -p build && cd build && cmake  -DCMAKE_BUILD_TYPE=Debug ..

clean:
	rm -fr build .vscode

test:
	./build/bitTest

debug:
	gdb -d ${PWD} ${PWD}/build/bitTest