BUILD_TYPE?=Release
BUILD_DIR?=build

default: configure make

debug:
	$(eval BUILD_TYPE := Debug)

cmake_check:
	@cmake --version || (echo "\n** Please install 'cmake' **\n" && exit 1)

configure: cmake_check
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ../src
	@echo "Modify build configuration in '$(BUILD_DIR)'"

make:
	cd $(BUILD_DIR) && make

install:
	cd $(BUILD_DIR) && make install

clean:
	rm -r $(BUILD_DIR) || true
	rm tags || true

all: clean default install

tags:
	ctags * -R src
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q /home/safl/git/rocksdb

dev: clean configure make
	./$(BUILD_DIR)/e01
