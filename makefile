
ROOT_DIR:=$(PWD)
BUILD_TYPE:="Release"
BUILD_DIR=$(ROOT_DIR)/build
CONAN_MARKER=$(BUILD_DIR)/conanbuildinfo.cmake

#-------------------------------------------------------------------
#  Part 2: Invoke the call to make in the build directory
#-------------------------------------------------------------------
.PHONY: build
build: cmake
	cd $(BUILD_DIR) && \
		make

.PHONY: conan
conan: $(CONAN_MARKER)S

$(CONAN_MARKER): 
	@ echo "did not find conan-build-info file... rebuilding: "
	cd $(BUILD_DIR) && conan install ..

cmake: $(CONAN_MARKER)
	cd $(BUILD_DIR) && \
		cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..

clean: 
	rm -rf build/*

.PHONY: help
help: 
	@ echo "Notes:"
	@ echo "....nyi"

.PHONY: test
test: testall

testall: build
	clear
	build/bin/testall

