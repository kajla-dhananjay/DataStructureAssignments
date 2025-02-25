BUILD_DIR=build

all: clean build run plot

.PHONY: all build run plot installer clean

build:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DDebug=ON .. && cmake --build . && cd -

run:
	./$(BUILD_DIR)/main/main_exe > out.txt

plot: installer
	python3 postprocess.py

installer:
	./install_dependencies.sh


clean:
	rm -rf $(BUILD_DIR) out.txt plots
