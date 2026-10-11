.PHONY: cmake build test clean

cmake: clean
	cmake -S . -B build

build: cmake
	cmake --build build

test: build
	ctest --test-dir build --output-on-failure

clean:
	rm -rf build