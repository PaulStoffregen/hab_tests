#ifndef TRACE_FEATURE_CFG_H
#define TRACE_FEATURE_CFG_H


#define TRC_FEAT_DLCF_ENA    STD_OFF
#define TRC_FEAT_BCP_ENA     STD_OFF
#define TRC_FEAT_FBLCTL_ENA  STD_OFF
#define TRC_FEAT_UPDSEQ_ENA  STD_OFF


/*! 
 * Define the trace features to be passed to the
 * TRACE_FEATURE() macro
 */
enum TRC_FEAT_ID
{
  TRC_FEAT_ID_eDLCF = 0,
  TRC_FEAT_ID_eBCP,
  TRC_FEAT_ID_eFBLCTL,
  TRC_FEAT_ID_eUPDSEQ,
  MAX_TRC_FEATURE_ID, /* Number of trace features */
};


/* List of trace features */
#ifdef TRACE_CORE_C
const char* trc_featList[MAX_TRC_FEATURE_ID] =
{
  [TRC_FEAT_ID_eDLCF] =   "DLCF",
  [TRC_FEAT_ID_eBCP] =    "BCP",
  [TRC_FEAT_ID_eFBLCTL] = "FBLCTL",
  [TRC_FEAT_ID_eUPDSEQ] = "UPDSEQ",
};
#endif /* TRACE_CORE_C */


/*!
 * Define the initial feature trace classes for feature
 */
#if (TRC_FEAT_DLCF_ENA == STD_ON)
#define TRC_FEAT_DLCF_INIT        TRACE_FEATURE_CLASS2
#else /* (TRC_FEAT_DLCF_ENA != STD_ON) */
#define TRC_FEAT_DLCF_INIT        0
#endif /* (TRC_FEAT_DLCF_ENA) */

#if (TRC_FEAT_BCP_ENA == STD_ON)
#define TRC_FEAT_BCP_INIT         TRACE_FEATURE_CLASS2
#else /* (TRC_FEAT_BCP_ENA != STD_ON) */
#define TRC_FEAT_BCP_INIT         0
#endif /* (TRC_FEAT_BCP_ENA) */

#if (TRC_FEAT_FBLCTL_ENA == STD_ON)
#define TRC_FEAT_FBLCTL_INIT      (TRACE_FEATURE_CLASS2)
#else /* (TRC_FEAT_FBLCTL_ENA != STD_ON) */
#define TRC_FEAT_FBLCTL_INIT      0
#endif /* (TRC_FEAT_FBLCTL_ENA) */

#if (TRC_FEAT_UPDSEQ_ENA == STD_ON)
#define TRC_FEAT_UPDSEQ_INIT      TRACE_FEATURE_CLASS2
#else /* (TRC_FEAT_UPDSEQ_ENA != STD_ON) */
#define TRC_FEAT_UPDSEQ_INIT      0
#endif /* (TRC_FEAT_UPDSEQ_ENA) */

/* Initial feature trace mask */
#define TRACE_FEATURE_MASK_INIT \
{\
  [TRC_FEAT_ID_eDLCF] =   TRC_FEAT_DLCF_INIT, \
  [TRC_FEAT_ID_eBCP] =    TRC_FEAT_BCP_INIT, \
  [TRC_FEAT_ID_eFBLCTL] = TRC_FEAT_FBLCTL_INIT, \
  [TRC_FEAT_ID_eUPDSEQ] = TRC_FEAT_UPDSEQ_INIT, \
}

#endif /* TRACE_FEATURE_CFG_H */
