
#IP address of the drone
IP=192.168.10.2

ifeq ($(shell uname -m),x86_64)
ARCHSUFFIX=-x64
endif

CCPREFIX=$(BASEDIR)/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian$(ARCHSUFFIX)/bin/arm-linux-gnueabihf-

SYSROOT=$(BASEDIR)/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/sysroot

export CCPREFIX
export CC=$(CCPREFIX)gcc --sysroot=$(SYSROOT)
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

$(BINARIES): setup
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
