#ifndef CPU_ASM_H
#define CPU_ASM_H

void cpu_goToSleep(void); //TODO: [FM] Move to cpu_sleep.h/.c(.S)

void cpu_memset4(uint32* dstStart, uint32 pattern, uint32* dstEnd);
void cpu_memcpy4(uint32* dstStart, uint32* srcStart, uint32* dstEnd);
void cpu_memchk4(uint32* memStart, uint32 pattern, uint32* memEnd);


#endif /* CPU_ASM_H */

