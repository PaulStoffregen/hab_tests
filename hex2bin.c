// Read a HEX file and output binary data
// Add the CSF address into the IVT (at address 0x60001018)
// Print the required .CSF "[Authenticate Data] Blocks =" line

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include "ihex.h"

#define PADDING 4096

void die(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

void usage(void)
{
	die("Usage: hex2bin file.hex\n");
}

int main(int argc, char **argv)
{
	const char *filename;
	char outname[256];
	unsigned int addr, len, mod, csf;
	FILE *fp;
	int r;

	// get input and output filenames
	if (argc < 2) usage();
	filename = argv[1];
	len = strlen(filename);
	if (len < 5 || len > sizeof(outname)-5) usage();
	printf("suffix = %s\n", filename + len - 4);
	if (strcmp(filename + len - 4, ".hex") != 0) usage();
	memset(outname, 0, sizeof(outname));
	memcpy(outname, filename, len - 4);
	strcat(outname, ".bin");
	printf("outname = %s\n", outname);

	// read the hex data
	r = ihex_read_file(filename);
	if (r < 0) die("error reading %s\n", filename);
	printf("%s loaded, %d bytes\n", filename, r);
	//r = ihex_get_size(addr, 0x40000);
	addr = 0x1000;
	while (!memory_is_undefined(addr)) addr++;
	printf("Program data from %X to %X\n", 0x1000, addr);
	mod = addr % PADDING;
	if (mod) {
		addr += PADDING - mod;
		printf("add %u padding bytes, end addr %x\n", PADDING - mod, addr);
	}
	len = addr;

	// put the CSF address into the IVT
	csf = 0x60000000 + len;
	memory_write(0x1018, csf >> 0);
	memory_write(0x1019, csf >> 8);
	memory_write(0x101A, csf >> 16);
	memory_write(0x101B, csf >> 24);

	// write binary data output
	fp = fopen(outname, "wb");
	if (fp == NULL) die("unable to write %s\n", outname);
	for (addr=0; addr < len; addr++) {
		unsigned char b = memory_byte(addr);
		fwrite(&b, 1, 1, fp);
	}
	fclose(fp);

	printf("\n[Authenticate Data]\n");
	printf("\tVerification Index = 0\n");
	printf("\tBlocks = 0x60001000 4096 %u \"%s\"\n", len - 0x1000, outname);
	printf("\n");



	return 0;
}



void die(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}



