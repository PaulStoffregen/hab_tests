#ifndef HAB_PRV_H
#define HAB_PRV_H

extern T_HAB_IVT* ivt;
extern T_HAB_CSF* csf;

extern unsigned* __dcd_start;
extern unsigned* __dcd_end;

extern const T_HAB_RVT* hab_getRvt(void);

#endif /* HAB_PRV_H */

