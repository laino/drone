
BCMVERSION=1.42

LIB_DIRS=bcm2835

CFLAGS=-static -Wall -I. $(foreach dir,$(LIB_DIRS),$(wildcard $(dir)/*.c))

LDFLAGS=-lm 

IN=$(LIB_DIRS) $(wildcard *.c) $(wildcard *.h)

OUT=build/out

all: $(OUT)

bcm2835:
	@wget http://www.airspayce.com/mikem/bcm2835/bcm2835-$(BCMVERSION).tar.gz\
				-O build/bcm2835-$(BCMVERSION).tar.gz
	@cd build/ && tar xvf bcm2835-$(BCMVERSION).tar.gz
	@mkdir bcm2835
	@cp build/bcm2835-$(BCMVERSION)/src/bcm2835.{h,c} bcm2835

setup: bcm2835
	@mkdir -p $$(dirname $(OUT))

clean:
	rm -r build/*
		
$(OUT): setup $(IN)
	$(CC) $(CFLAGS) control.c -o $(OUT) $(LDFLAGS)

.PHONY: setup all clean
