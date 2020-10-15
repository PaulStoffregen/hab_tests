#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void die(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

void usage(void)
{
        die("Usage: ./srkfuse cst/keys/SRK_fuse.bin\n");
}

int main(int argc, char **argv)
{
	const char *filename;
	unsigned char buf[32];
	unsigned int i, n;
	struct stat s;
	FILE *fp;

	if (argc < 2) usage();
        filename = argv[1];
	if (stat(filename, &s) != 0) die("unable to find file %s\n", filename);
	if (s.st_size != 32) die("file %s is wrong size (%lu bytes), must be 32\n",
		filename, s.st_size);
	fp = fopen(filename, "rb");
	if (fp == NULL) die("unable to read %s\n", filename);
	if (fread(buf, 1, 32, fp) != 32) die("unable to read %s\n", filename);
	fclose(fp);

	for (i=0; i < 32; i += 4) {
		n = buf[i] | (buf[i+1] << 8) | (buf[i+2] << 16) | (buf[i+3] << 24);
		printf("fuse_write(&HW_OCOTP_SRK%u, 0x%08X);\n", i / 4, n);

	}

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

