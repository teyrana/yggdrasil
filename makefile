
ROOT_DIR:=$(PWD)
BUILD_TYPE:="Release"
BUILD_DIR=$(ROOT_DIR)/build
CONAN_MARKER=$(BUILD_DIR)/conanbuildinfo.cmake
TEST_EXE=build/bin/testall

#-------------------------------------------------------------------
#  Part 2: Invoke the call to make in the build directory
#-------------------------------------------------------------------
.PHONY: build
build: debug 

.PHONY: debug
debug: cmake-debug
	cd $(BUILD_DIR) && ninja

.PHONY: release
release: cmake-release
	cd $(BUILD_DIR) && ninja

.PHONY: conan
conan: $(CONAN_MARKER)S

$(CONAN_MARKER): 
	@ echo "did not find conan-build-info file... rebuilding: "
	cd $(BUILD_DIR) && conan install --build=missing ..

cmake-debug: $(CONAN_MARKER)
	cd $(BUILD_DIR) && \
		cmake .. -DCMAKE_BUILD_TYPE=Debug -GNinja

cmake-release: $(CONAN_MARKER)
	cd $(BUILD_DIR) && \
		cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja

clean: 
	rm -rf build/*

.PHONY: help
help: 
	@ echo "Notes:"
	@ echo "....nyi"

.PHONY: test testgrid
test: testall
gridtest: debug
	$(TEST_EXE) --gtest_filter=FixedGrid*

quadtest: debug
	$(TEST_EXE) --gtest_filter=QuadTree*

searchtest: debug
	$(TEST_EXE) --gtest_filter=Search*

testall: build
	clear
	$(TEST_EXE)

