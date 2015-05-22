BCMVERSION=1.42

LIB_DIRS=bcm2835

GCCOPTIMIZATIONS=-O3 -march=armv6 -mfpu=vfp -mfloat-abi=hard
CFLAGS=-Wall -static $(GCCOPTIMIZATIONS) -I. $(foreach dir,$(LIB_DIRS),$(wildcard $(dir)/*.c))

LDFLAGS=-lm -lrt

IN=$(LIB_DIRS) $(wildcard *.c) $(wildcard *.h)

OUT=build/out

all: $(OUT)

bcm2835:
	@mkdir -p build
	@wget http://www.airspayce.com/mikem/bcm2835/bcm2835-$(BCMVERSION).tar.gz\
				-O build/bcm2835-$(BCMVERSION).tar.gz
	@cd build/ && tar xvf bcm2835-$(BCMVERSION).tar.gz
	@mkdir -p bcm2835
	@cp build/bcm2835-$(BCMVERSION)/src/bcm2835.{h,c} bcm2835

clean:
	rm -r build/*
		
$(OUT): bcm2835 $(IN)
	$(CC) $(CFLAGS) control.c -o $(OUT) $(LDFLAGS)

.PHONY: all clean
