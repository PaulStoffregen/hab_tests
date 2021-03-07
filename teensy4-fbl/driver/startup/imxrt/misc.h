#ifndef MISC_H
#define MISC_H


void earlyDbg_init(void);
void earlyDbg_sendChar(char c);
void earlyDbg_sendStr(const char* str);

void ldr_dumpLinkerInfo(void);
void boot_dumpBootInfo(void);

extern void (*imxrt_ram_vectors[IRQ_ID_MAX_IRQS + 16])(void);
extern void armv7m_default_handler(void);

#endif /* MISC_H */

