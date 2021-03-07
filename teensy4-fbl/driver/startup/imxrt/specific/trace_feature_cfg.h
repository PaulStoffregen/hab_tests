#ifndef TRACE_FEATURE_CFG_H
#define TRACE_FEATURE_CFG_H

/*! 
 * Define the trace features to be passed to the
 * TRACE_FEATURE() macro
 */
enum TRC_FEAT_ID
{
  /* TRC_FEAT_ID_eXXX = 0, */
  MAX_TRC_FEATURE_ID = 0, /* Number of trace features */
};


/* List of trace features */
#ifdef TRACE_CORE_C
char* trc_featList[MAX_TRC_FEATURE_ID] =
{
  /* [TRC_FEAT_ID_eXXX] = "XXX", */
};
#endif /* TRACE_CORE_C */


/*!
 * Define the initial feature trace classes for feature
 */
#if (TRC_FEAT_XXX_ENA == STD_ON)
#define TRC_FEAT_XXX_INIT        TRACE_FEATURE_CLASS1
#else
#define TRC_FEAT_XXX_INIT        0
#endif /* (TRC_FEAT_XXX_ENA) */


/* Initial feature trace mask */
#define TRACE_FEATURE_MASK_INIT \
{\
  /* [TRC_FEAT_ID_eXXX] = TRC_FEAT_XXX_INIT, */\
}

#endif /* TRACE_FEATURE_CFG_H */
