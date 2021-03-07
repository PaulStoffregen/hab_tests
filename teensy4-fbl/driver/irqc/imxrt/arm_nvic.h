#ifndef ARM_NVIC_H
#define ARM_NVIC_H

extern void nvic_setPriorityGrouping(uint8 priGrp);
extern uint8 nvic_getPriorityGrouping(void);

extern void nvic_enableIrq(sint16 irqNum);
extern void nvic_disableIrq(sint16 irqNum);
extern void nvic_setIrqPending(sint16 irqNum);
extern void nvic_clrIrqPending(sint16 irqNum);

extern boolean nvic_isIrqPending(sint16 irqNum);
extern boolean nvic_isIrqActive(sint16 irqNum);

extern void nvic_setIrqPrio(sint16 irqNum, uint32 irqPrio);
extern uint32 nvic_getIrqPrio(sint16 irqNum);

extern uint32 nvic_encodeIrqPrio(uint32 grpPrio, uint32 subPrio, uint32 priGrp);
extern void nvic_decodeIrqPrio(uint32* grpPrio, uint32* subPrio, uint32 priGrp, uint32 irqPrio);

#endif /* ARM_NVIC_H */

