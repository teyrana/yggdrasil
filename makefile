
ROOT_DIR:=$(PWD)
SRC_ROOT:=$(ROOT_DIR)/src

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
debug: BUILD_TYPE="Debug"
debug: help configure #build
	cd $(BUILD_DIR) && ninja

.PHONY: release
release: BUILD_TYPE="Debug"
release: configure build
	cd $(BUILD_DIR) && ninja

.PHONY: conan
conan: $(CONAN_MARKER)

$(CONAN_MARKER): 
	@ echo "did not find conan-build-info file... rebuilding: "
	cd $(BUILD_DIR) && conan install --build=missing --profile=gcc-profile ..

configure: $(CONAN_MARKER)
	cd $(BUILD_DIR) && \
		cmake .. -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -GNinja

clean: 
	rm -rf build/*

format:
	@echo clang-tidy src

.PHONY: help
help: 
	@ echo "Notes:"
	@ echo "....nyi"

.PHONY: test testgrid
test: test-all
test-fix-grid: debug
	$(TEST_EXE) --gtest_filter=FixedGrid*

test-roll-grid: debug
	$(TEST_EXE) --gtest_filter=RollingGrid*

test-quad-tree: debug
	$(TEST_EXE) --gtest_filter=QuadTree*

test-search: debug
	$(TEST_EXE) --gtest_filter=Search*

test-all: build
	clear
	$(TEST_EXE)

