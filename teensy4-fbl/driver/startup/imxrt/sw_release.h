#ifndef SW_RELEASE_H
#define SW_RELEASE_H

#define SW_REL_LAYOUT_ID     0
#define SW_REL_MAX_VER_SIZE 12

extern const char sw_rel_date[];
extern const char sw_rel_time[];
extern const uint32 sw_rel_rev;
extern const char sw_rel_flags[];
//extern const char sw_rel_rurl[];
extern const char sw_rel_rev_str[SW_REL_MAX_VER_SIZE];


typedef struct
{
  uint8  layoutID[2];
  uint8  major[2];
  uint8  minor[2];
  uint8  patch[2];
  uint8  revision[4];
  uint8  shortHash[8];
  uint8  relDate[11];   /* JJJJ-MM-TT */
  uint8  relTime[9];    /* HH:MM:SS */
  uint8  buildDate[11]; /* JJJJ-MM-TT */
  uint8  buildTime[9];  /* HH:MM:SS */
}T_SW_REL_INFO;


extern const T_SW_REL_INFO sw_rel_data;

#endif /* SW_RELEASE_H */

