#include <stdio.h>
#include <string.h>
#include "ihex.h"

/****************************************************************/
/*                                                              */
/*                     Read Intel Hex File                      */
/*                                                              */
/****************************************************************/

#define MAX_MEMORY_SIZE 0x40000
static unsigned char firmware_data[MAX_MEMORY_SIZE];
static unsigned char firmware_mask[MAX_MEMORY_SIZE];
static int end_record_seen=0;
static int byte_count;
static unsigned int extended_addr = 0;
static int parse_hex_line(char *line);

int ihex_read_file(const char *filename)
{
	FILE *fp;
	int lineno=0;
	char buf[1024];

	byte_count = 0;
	end_record_seen = 0;

	memset(firmware_data, 0xFF, MAX_MEMORY_SIZE);
	memset(firmware_mask, 0, MAX_MEMORY_SIZE);

	extended_addr = 0;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		//printf("Unable to read file %s\n", filename);
		return -1;
	}
	while (!feof(fp)) {
		*buf = '\0';
		if (!fgets(buf, sizeof(buf), fp)) break;
		lineno++;
		if (*buf) {
			if (parse_hex_line(buf) == 0) {
				printf("Warning, HEX parse error line %d\n", lineno);
				return -2;
			}
		}
		if (end_record_seen) break;
		if (feof(stdin)) break;
	}
	fclose(fp);
	return byte_count;
}

static int store_byte(unsigned int address, unsigned char data)
{
	if (address < MAX_MEMORY_SIZE) {
		firmware_data[address] = data;
		firmware_mask[address] = 1;
		return 1;
	}
	return 0;
}

/* from ihex.c, at http://www.pjrc.com/tech/8051/pm2_docs/intel-hex.html */

/* parses a line of intel hex code, stores the data in bytes[] */
/* and the beginning address in addr, and returns a 1 if the */
/* line was valid, or a 0 if an error occured.  The variable */
/* num gets the number of bytes that were stored into bytes[] */


static int
parse_hex_line(char *line)
{
	int addr, code, num;
        int sum, len, cksum, i;
        char *ptr;
        
        num = 0;
        if (line[0] != ':') return 0;
        if (strlen(line) < 11) return 0;
        ptr = line+1;
        if (!sscanf(ptr, "%02x", &len)) return 0;
        ptr += 2;
        if ((int)strlen(line) < (11 + (len * 2)) ) return 0;
        if (!sscanf(ptr, "%04x", &addr)) return 0;
        ptr += 4;
          //printf("Line: length=%d Addr=%d\n", len, addr);
        if (!sscanf(ptr, "%02x", &code)) return 0;
	//if (addr + extended_addr + len >= MAX_MEMORY_SIZE) return 0;
        ptr += 2;
        sum = (len & 255) + ((addr >> 8) & 255) + (addr & 255) + (code & 255);
	if (code != 0) {
		if (code == 1) {
			end_record_seen = 1;
			return 1;
		}
		if (code == 2 && len == 2) {
			if (!sscanf(ptr, "%04x", &i)) return 1;
			ptr += 4;
			sum += ((i >> 8) & 255) + (i & 255);
        		if (!sscanf(ptr, "%02x", &cksum)) return 1;
			if (((sum & 255) + (cksum & 255)) & 255) return 1;
			extended_addr = i << 4;
			if (extended_addr >= 0x60000000) extended_addr -= 0x60000000;
			//printf("ext addr = %05X\n", extended_addr);
		}
		if (code == 4 && len == 2) {
			if (!sscanf(ptr, "%04x", &i)) return 1;
			ptr += 4;
			sum += ((i >> 8) & 255) + (i & 255);
        		if (!sscanf(ptr, "%02x", &cksum)) return 1;
			if (((sum & 255) + (cksum & 255)) & 255) return 1;
			extended_addr = i << 16;
			if (extended_addr >= 0x60000000) extended_addr -= 0x60000000;
			//printf("ext addr = %08X\n", extended_addr);
		}
		return 1;	// non-data line
	}
	byte_count += len;
        while (num != len) {
                if (sscanf(ptr, "%02x", &i) != 1) return 0;
		i &= 255;
		if (!store_byte(addr + extended_addr + num, i)) {
			printf("Error, no memory for data at addr=%08X\n",
				addr + extended_addr + num);
		}
                ptr += 2;
                sum += i;
                (num)++;
                if (num >= 256) return 0;
        }
        if (!sscanf(ptr, "%02x", &cksum)) return 0;
        if (((sum & 255) + (cksum & 255)) & 255) return 0; /* checksum error */
        return 1;
}

#if 1
int ihex_bytes_within_range(int begin, int len)
{
	int i, end;

	end = begin + len - 1;
	if (begin < 0 || begin >= MAX_MEMORY_SIZE ||
	   end < 0 || end >= MAX_MEMORY_SIZE) {
		return 0;
	}
	for (i=begin; i<=end; i++) {
		if (firmware_mask[i]) return 1;
	}
	return 0;
}
#endif

/*int ihex_get_size(unsigned int address, int len)
{
	unsigned int size = 0;
	unsigned int end = begin + len - 1;
	unsigned int i;

	if (end >= MAX_MEMORY_SIZE) end = MAX_MEMORY_SIZE-1;
	for (i=begin; i<=end; i++) {
		if (firmware_mask[i]) {
			//printf(" mask 1 at %d\n", i);
			size = i + 1;
		}
	}
	return size - begin;
}*/


void ihex_get_data(int addr, int len, unsigned char *bytes)
{
	int i;

	if (addr < 0 || len < 0 || addr + len >= MAX_MEMORY_SIZE) {
		for (i=0; i<len; i++) {
			bytes[i] = 255;
		}
		return;
	}
	for (i=0; i<len; i++) {
		if (firmware_mask[addr]) {
			bytes[i] = firmware_data[addr];
		} else {
			bytes[i] = 255;
		}
		addr++;
	}
}

int memory_is_undefined(int addr)
{
	if (addr < 0 || addr > MAX_MEMORY_SIZE) return 1;
	if (firmware_mask[addr] == 0) return 1;
	return 0;
}

unsigned char memory_byte(int addr)
{
	if (memory_is_undefined(addr)) return 0xFF;
	return firmware_data[addr];
}


void memory_write(int addr, unsigned char byte)
{
	if (addr < 0 || addr > MAX_MEMORY_SIZE) return;
	firmware_data[addr] = byte;
	firmware_mask[addr] = 1;
}

