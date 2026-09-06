build: clean
	go build .

test: clean
	go build . && go test .

clean:
	go clean

include $(shell git rev-parse --show-toplevel)/visualizer/visualize.mk