PREFIX=/usr
CC = cc
CFLAGS = -Wall -O2 -I$(PREFIX)/include
LDFLAGS = -L$(PREFIX)/lib

all: pdftxt
%.o: %.c
	$(CC) -c $(CFLAGS) $<
pdftxt: pdftxt.o
	$(CC)  -o $@ $^ $(LDFLAGS) -lmupdf -lmupdf-third -lm -lz -lpthread \
		-lfreetype -lharfbuzz -ljbig2dec -lopenjp2 -ljpeg
clean:
	rm -f *.o pdftxt
