///////////////////////////////////////////////////////////////////////
////  Copyright 2015 Samsung Austin Semiconductor, LLC.                //
/////////////////////////////////////////////////////////////////////////
//
#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include "utils.h"


#define LPT_CTR_MAX  7
#define LPT_CTR_INIT 4

#define GCPT_CTR_MAX  3
#define GCPT_CTR_INIT 2

#define HIST_LEN   11
#define GLOBAL_HIST_LEN 14

class PREDICTOR{

  // The state is defined for Gshare, change for your design

 private:
  UINT32  ghr;           // global history register
  UINT32  *lht;          // local history table
  UINT32  *lpt;          // local prediction table
  UINT32  *gpt;          // global prediction table
  UINT32  *cpt;          // choice prediction table
  UINT32  historyLength; // history length
  UINT32  globalHistoryLength;  //global history length
  UINT32  numLptEntries; // entries in lht and lpt
  UINT32  numGCptEntries; // entries in gpt and cpt

 public:
  PREDICTOR(void);
  bool    GetPrediction(UINT64 PC);  
  void    UpdatePredictor(UINT64 PC, OpType opType, bool resolveDir, bool predDir, UINT64 branchTarget);
  void    TrackOtherInst(UINT64 PC, OpType opType, bool branchDir, UINT64 branchTarget);
  UINT32  SatIncrement(UINT32 counter,bool type);
  UINT32  SatDecrement(UINT32 counter);
};

/////////////// STORAGE BUDGET JUSTIFICATION ////////////////
// Total LHT size = 2^11 * 11 bits/entry  = 22 Kilo bits
// Total LPT counters: 2^11 
// Total LPT size = 2^11 * 3 bits/counter = 6  Kilo bits
// GHR size = 14 bits
// Total GPT size = 2^14 * 2 bits/counter = 32 Kilo bits
// Total CPT size = 2^14 * 2 bits/counter = 32 Kilo bits
// Total Size = LHT size + LPT size + GPT size + CPT size + GHR = 92 Kilo bits + 14 bits
// Total Size(in Bytes) = 11.50175 KiloBytes 
/////////////////////////////////////////////////////////////



#endif

