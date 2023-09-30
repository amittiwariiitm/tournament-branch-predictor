#include "predictor.h"

PREDICTOR::PREDICTOR(void){

  historyLength       = HIST_LEN;
  globalHistoryLength = GLOBAL_HIST_LEN;
  ghr                 = 0;
  numLptEntries       = (1<< HIST_LEN);
  numGCptEntries      = (1<< GLOBAL_HIST_LEN);

  lht = new UINT32[numLptEntries];
  lpt = new UINT32[numLptEntries];
  gpt = new UINT32[numGCptEntries];
  cpt = new UINT32[numGCptEntries];

  for(UINT32 ii=0; ii< numLptEntries; ii++){
    lht[ii] = 0;
    lpt[ii] = LPT_CTR_INIT; 
  }

  for(UINT32 ii=0; ii< numGCptEntries; ii++){
    gpt[ii] = GCPT_CTR_INIT;
    cpt[ii] = GCPT_CTR_INIT; 
  }
  
}

UINT32  PREDICTOR::SatIncrement(UINT32 counter,bool type){
  if(type){
    if(counter < GCPT_CTR_MAX)
      counter++;
  }
  else{
    if(counter < LPT_CTR_MAX)
      counter++;
  }
  return counter;
}

UINT32  PREDICTOR::SatDecrement(UINT32 counter){
  if(counter > 0)
    counter--;
  return counter;
}

bool   PREDICTOR::GetPrediction(UINT64 PC){

  UINT32 mask       = (1<<historyLength)-1;
  UINT32 lhtIndex   = (PC & mask);
  UINT32 lptIndex   = lht[lhtIndex];
  UINT32 lptCounter = lpt[lptIndex];
  UINT32 gptCounter = gpt[ghr];
  UINT32 cptCounter = cpt[ghr];
  bool   flag1 = false, flag2 = false;

  if(lptCounter > (LPT_CTR_MAX/2)){ 
    flag1 = true; 
  }
  if(gptCounter > (GCPT_CTR_MAX/2)){ 
    flag2 = true; 
  }
  if(flag1 == flag2){
    return flag1;
  }
  else{
    if(cptCounter > (GCPT_CTR_MAX/2)){ 
        return flag2;
    }
  }
  return flag1;
}



void  PREDICTOR::UpdatePredictor(UINT64 PC, OpType opType, bool resolveDir, bool predDir, UINT64 branchTarget){

  UINT32 mask       = (1<<historyLength)-1;
  UINT32 ghrMask    = (1<<globalHistoryLength)-1;
  UINT32 lhtIndex   = (PC & mask);
  UINT32 lptIndex   = lht[lhtIndex];
  UINT32 gptIndex   = ghr;
  UINT32 lptCounter = lpt[lptIndex];
  UINT32 gptCounter = gpt[ghr];
  UINT32 cptCounter = cpt[ghr];
  bool   flag1 = false, flag2 = false;

  if(lptCounter > (LPT_CTR_MAX/2)){ 
    flag1 = true; 
  }
  if(gptCounter > (GCPT_CTR_MAX/2)){ 
    flag2 = true; 
  }
  if(flag1 != flag2){
    if(flag1 == resolveDir){
      cpt[gptIndex] = SatDecrement(cptCounter);
    }
    else{
      cpt[gptIndex] = SatIncrement(cptCounter,true);
    }
  }

  lht[lhtIndex] = (lht[lhtIndex] << 1);
  ghr = (ghr << 1);

  if(resolveDir == true){
    lpt[lptIndex] = SatIncrement(lptCounter, false);
    gpt[gptIndex] = SatIncrement(gptCounter, true);
    lht[lhtIndex]++;
    ghr++;
  }
  else{
    lpt[lptIndex] = SatDecrement(lptCounter);
    gpt[gptIndex] = SatDecrement(gptCounter);    
  }

  lht[lhtIndex] = lht[lhtIndex] & mask;
  ghr = ghr & ghrMask;
  

}

void PREDICTOR::TrackOtherInst (UINT64 PC, OpType opType, bool taken, UINT64 branchTarget){

}