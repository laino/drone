
#IP address of the drone
IP=192.168.10.2

CCPREFIX=$(BASEDIR)/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-

export CCPREFIX
export CC=$(CCPREFIX)gcc
export BASEDIR

BINARIES=remote control

BIN_DIR=bin

setup:
	@mkdir -p build/root/$(BIN_DIR)/

all: setup $(BINARIES)

install: all
	@echo " - Transferring files to the drone"
	@./transfer.sh $(IP)

clean:
	@rm -rf build/*
	@for bin in $(BINARIES); do\
		$(MAKE) -C $$bin clean;\
	done

$(BINARIES):
	@if [ -z "$(BASEDIR)" ]; then\
		echo "ERROR: Please specify the build tools BASEDIR. Example: \"BASEDIR=*path* make\"";\
		echo "       BASEDIR should refer to the root of this git repository:";\
		echo "       https://github.com/raspberrypi/tools";\
		echo "       Clone it, set BASEDIR to the root of your local clone and try again.";\
		exit 1;\
	fi;
	@echo " - Building $@"
	@$(MAKE) -C $@
	@cp $@/build/out build/root/$(BIN_DIR)/$@

.PHONY: all clean setup install $(BINARIES)
