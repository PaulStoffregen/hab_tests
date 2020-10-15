#ifndef _ihex_h_
#define _ihex_h_

int ihex_read_file(const char *filename);
int ihex_bytes_within_range(int begin, int len);
void ihex_get_data(int addr, int len, unsigned char *bytes);
int ihex_get_size(unsigned int addr, int len);
int memory_is_blank(int addr, int block_size);
int memory_is_undefined(int addr);
unsigned char memory_byte(int addr);
void memory_write(int addr, unsigned char byte);



#endif
