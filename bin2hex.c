#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void print_hex(FILE *fp, unsigned int addr, const unsigned char *data, unsigned int len);
void usage(void);
void die(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

unsigned char buf[0x40000];

void usage(void)
{
	die("Usage: hex2bin file.bin\n");
}

int main(int argc, char **argv)
{
        const char *filename;
        char outname[256];
	unsigned int len, n, offset=0;
	struct stat s;
	FILE *fp;

	// get input and output filenames
	if (argc < 2) usage();
	filename = argv[1];
	len = strlen(filename);
	if (len < 5 || len > sizeof(outname)-5) usage();
	printf("suffix = %s\n", filename + len - 4);
	if (strcmp(filename + len - 4, ".bin") != 0) usage();
	memset(outname, 0, sizeof(outname));
	memcpy(outname, filename, len - 4);
	strcat(outname, ".hex");
	printf("outname = %s\n", outname);

	if (stat(filename, &s) != 0) die("unable to find file %s\n", filename);
	len = s.st_size;
	printf("file size = %u\n", len);
	if (len > sizeof(buf)) die("error, file is too large\n");
	fp = fopen(filename, "rb");
	if (fp == NULL) die("unable to read %s\n", filename);
	if (fread(buf, 1, len, fp) != len) die("unable to read %s\n", filename);
	fclose(fp);

	fp = fopen(outname, "w");
	if (fp == NULL) die("unable to write %s\n", outname);
	while (offset < len) {
		n = len - offset;
		if (n > 32) n = 32;
		print_hex(fp, offset, buf + offset, n);
		offset += n;
	}
	fprintf(fp, ":00000001FF\n");
	fclose(fp);

	return 0;
}

void print_hex(FILE *fp, unsigned int addr, const unsigned char *data, unsigned int len)
{
	static unsigned int extaddr = 0xFFFFFFFF;
	unsigned int n, sum;

	if ((addr & 0xFFFF0000) != extaddr) {
		extaddr = addr & 0xFFFF0000;
		//printf("extended address %08X\n", extaddr);
		n = ((extaddr >> 16) + 0x6000) & 0xFFFF;
		sum = 2 + 4 + (n >> 8) + n;
		sum = (~sum + 1) & 0xFF;
		fprintf(fp, ":02000004%04X%02X\n", n, sum);
	}
	//printf("%04X: %d bytes\n", addr, len);
	sum = len + (addr >> 8) + addr;
	fprintf(fp, ":%02X%04X00", len, addr & 0xFFFF);
	for (n=0; n < len; n++) {
		fprintf(fp, "%02X", data[n]);
		sum += data[n];
	}
	sum = (~sum + 1) & 0xFF;
	fprintf(fp, "%02X\n", sum);
}

void die(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}

