CC = gcc
CFLAGS = -O2 -Wall

all: hex2bin bin2hex srkfuse hex2csf

hex2csf: hex2csf.c ihex.o
	gcc $(CFLAGS) -o hex2csf hex2csf.c ihex.o

hex2bin: hex2bin.c ihex.o
	gcc $(CFLAGS) -o hex2bin hex2bin.c ihex.o

bin2hex: bin2hex.c
	gcc $(CFLAGS) -o bin2hex bin2hex.c

srkfuse: srkfuse.c
	gcc $(CFLAGS) -o srkfuse srkfuse.c

clean:
	rm *.o hex2bin bin2hex srkfuse

