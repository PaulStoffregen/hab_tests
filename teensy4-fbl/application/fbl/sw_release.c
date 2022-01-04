#ifndef SW_RELEASE_C
#define SW_RELEASE_C
#endif /* SW_RELEASE_C */


#include "bsp.h"
#include "version.h"
#include "sw_release.h"

#define __sw_rel_info_section __attribute__((section(".sw_rel_info")))

#define QUOTE(s) #s
#define QUOTE_AND_EXPAND(s) QUOTE(s)

#define UINT32_TO_BYTES(word) \
  (uint8)((word) >> 24), (uint8)((word) >> 16), (uint8)((word) >> 8), (uint8)((word) >> 0)

#define BYTES_TO_UINT32(b3, b2, b1, b0) \
  ((uint32) ( (((uint8)(b3)) << 24)     \
            | (((uint8)(b2)) << 16)     \
            | (((uint8)(b1)) <<  8)     \
            | (((uint8)(b0)) <<  0)))

#define UINT16_TO_BYTES(halfword) \
  (uint8)((halfword) >> 8), (uint8)((halfword) >> 0)

#define BYTES_TO_UINT16(b1, b0)         \
  ((uint16) ( (((uint8)(b1)) <<  8)     \
            | (((uint8)(b0)) <<  0)))


//static const __sw_rel_info_section char   sw_rel_rurl[] = QUOTE_AND_EXPAND(SW_REL_RURL);
const __sw_rel_info_section char   sw_build_time[] = QUOTE_AND_EXPAND(SW_BUILD_TIME);
const __sw_rel_info_section char   sw_build_date[] = QUOTE_AND_EXPAND(SW_BUILD_DATE);
const __sw_rel_info_section char   sw_rel_time[] = QUOTE_AND_EXPAND(SW_REL_TIME);
const __sw_rel_info_section char   sw_rel_date[] = QUOTE_AND_EXPAND(SW_REL_DATE);
const __sw_rel_info_section char   sw_rel_flags[] = QUOTE_AND_EXPAND(SW_REL_MODIFIED) ;
const __sw_rel_info_section uint32 sw_rel_rev = SW_REL_LASTREV;
const __sw_rel_info_section char   sw_rel_rev_str[SW_REL_MAX_VER_SIZE] = QUOTE_AND_EXPAND(SW_REL_REV_STR) QUOTE_AND_EXPAND(SW_REL_MODIFIED);


const __sw_rel_info_section T_SW_REL_INFO sw_rel_data =
{
  .layoutID = {UINT16_TO_BYTES(SW_REL_LAYOUT_ID)},
  .major = {UINT16_TO_BYTES(SW_REL_MAJOR)},
  .minor = {UINT16_TO_BYTES(SW_REL_MINOR)},
  .patch = {UINT16_TO_BYTES(SW_REL_PATCH)},
  .revision = {UINT32_TO_BYTES(SW_REL_LASTREV)},
  .shortHash = QUOTE_AND_EXPAND(SW_REL_HASH),
  .relDate = QUOTE_AND_EXPAND(SW_REL_DATE),
  .relTime = QUOTE_AND_EXPAND(SW_REL_TIME),
  .buildDate = QUOTE_AND_EXPAND(SW_BUILD_DATE),
  .buildTime = QUOTE_AND_EXPAND(SW_BUILD_TIME),
};

