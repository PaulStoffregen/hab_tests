// Read a HEX file and output CSF file and binary data

// Add the CSF address into the IVT (at address 0x60001018)
// Print the required .CSF "[Authenticate Data] Blocks =" line

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include "ihex.h"

void die(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

void usage(void)
{
	die("Usage: hex2csf file.hex\n");
}

int main(int argc, char **argv)
{
	const char *filename;
	char binname[256], csfname[256];
	unsigned int addr, len;
	unsigned int ivt_header, ivt_address, cfs_address;
	FILE *fp;
	int r;

	// get input and output filenames
	if (argc < 2) usage();
	filename = argv[1];
	len = strlen(filename);
	if (len < 5 || len > sizeof(binname)-5) usage();
	printf("suffix = %s\n", filename + len - 4);
	if (strcmp(filename + len - 4, ".hex") != 0) usage();
	memset(binname, 0, sizeof(binname));
	memcpy(binname, filename, len - 4);
	strcat(binname, ".bin");
	printf("binname = %s\n", binname);
	memset(csfname, 0, sizeof(csfname));
	memcpy(csfname, filename, len - 4);
	strcat(csfname, ".csf");
	printf("csfname = %s\n", csfname);

	// read the hex data
	r = ihex_read_file(filename);
	if (r < 0) die("error reading %s\n", filename);
	printf("%s loaded, %d bytes\n", filename, r);
	//r = ihex_get_size(addr, 0x40000);

	ivt_header = memory_word(0x1000);
	ivt_address = memory_word(0x1014);
	cfs_address = memory_word(0x1018);

	printf("ivt header = %08X\n", ivt_header);
	printf("ivt addr = %08X\n", ivt_address);
	printf("cfs addr = %08X\n", cfs_address);

	if (ivt_header != 0x432000D1) {
		die("Error: IVT header missing at 0x60001000, should be 432000D1, but is %08X\n",
			ivt_header);
	}
	if (ivt_address != 0x60001000) {
		die("Error: IVT address wrong 0x60001014, should be 60001000, but is %08X\n",
			ivt_address);
	}
	if (cfs_address <= ivt_address) {
		die("Error: CFS address must be greater than 60001000, but is %08X\n",
			cfs_address);
	}

	addr = 0x1000;
	while (!memory_is_undefined(addr)) addr++;
	printf("Data from %X to %X\n", 0x1000, addr);

	len = cfs_address - 0x60000000;

	// write binary data output
	fp = fopen(binname, "wb");
	if (fp == NULL) die("unable to write %s\n", binname);
	for (addr=0; addr < len; addr++) {
		unsigned char b = memory_byte(addr);
		fwrite(&b, 1, 1, fp);
	}
	fclose(fp);

	fp = fopen(csfname, "w");
	if (fp == NULL) die("unable to write %s\n", csfname);
	fprintf(fp, "[Header]\n");
	fprintf(fp, "\tVersion = 4.1\n");
	fprintf(fp, "[Install SRK]\n");
	fprintf(fp, "\tFile = \"/home/paul/teensy/arm/imxrt/encryption/hab_tests/key/SRK_table.bin\"\n");
	fprintf(fp, "\tSource Index = 0\n");
	fprintf(fp, "[Install NOCAK]\n");
	fprintf(fp, "\tFile = \"/home/paul/teensy/arm/imxrt/encryption/hab_tests/key/SRK1_sha256_2048_65537_v3_usr_crt.pem\"\n");
	fprintf(fp, "[Authenticate CSF]\n");
	fprintf(fp, "\tSignature Format = CMS\n");
	fprintf(fp, "\n[Authenticate Data]\n");
	fprintf(fp, "\tVerification Index = 0\n");
	fprintf(fp, "\tBlocks = 0x60001000 0x00001000 0x%08X \"%s\"\n",
		cfs_address - 0x60001000, binname);
	fprintf(fp, "\n");
	fclose(fp);

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



