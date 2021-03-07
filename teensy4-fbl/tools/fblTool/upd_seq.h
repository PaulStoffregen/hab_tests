#ifndef UPD_SEQ_H
#define UPD_SEQ_H

void updseq_init(const char* fileName);
void updseq_run(void);
void updseq_registerCallback(void (*newCbk)(uint32));
T_STATUS updseq_start(void);

#endif /* UPD_SEQ_H */

