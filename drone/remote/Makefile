CFLAGS = -Wall

IN=$(wildcard *.c) $(wildcard *.h)

OUT=build/out

all: $(OUT)

clean:
	rm $(OUT)
		
$(OUT): $(IN)
	mkdir -p $$(dirname $(OUT))
	$(CC) $(CFLAGS) remote.c -o $(OUT)

.PHONY: all clean
