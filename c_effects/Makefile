# Clean the build directory
clean:
	rm -rf build

# Build the project
build: clean
	cmake -B build
	cmake --build build

# Run the executable
run: build
	./build/MyCProj

# Clean, build and run
all: run