#ifndef IRQC_H
#define IRQC_H


typedef void (*T_IRQ_HANDLER)(void);

/* Type definition for interrupt configuration */
typedef struct
{
  sint16 irqNum;
  sint8  irqPrio;
  sint8  subPrio;
}T_IRQC_IRQ_CFG;

#define IRQC_CFG_IRQ(n, gp, sp) \
{                               \
  .irqNum = (n),                \
  .irqPrio = (gp),              \
  .subPrio = (sp),              \
}

#define IRQC_CFG_IRQ_END()      \
{                               \
  .irqNum = 0,                  \
  .irqPrio = -1,                \
  .subPrio = 0,                 \
}

extern void irqc_init(void);

extern void irqc_setupMultipleIrqs(const T_IRQC_IRQ_CFG* irqCfgArray);
extern void irqc_setupSingleIrq(const T_IRQC_IRQ_CFG* irqCfg);

extern void irqc_registerHandler(sint16 irqNum, T_IRQ_HANDLER irqHandler);


#endif /* IRQC_H */

