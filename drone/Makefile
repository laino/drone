
#IP address of the drone
IP=192.168.10.2

BINARIES=remote control

all: $(BINARIES)

install: all
	echo "Copying files to the drone via network..."
	./transfer.sh $(IP)

clean:
	rm -rf build/*

$(BINARIES):
	@$(MAKE) -C $@
	cp $@/build/out build/$@

.PHONY: all clean install $(BINARIES)
