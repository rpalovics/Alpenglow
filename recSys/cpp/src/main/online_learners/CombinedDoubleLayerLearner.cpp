#include "CombinedDoubleLayerLearner.h"

void CombinedDoubleLayerLearner::learn(RecDat * rec_dat){
  if(combination_learner_!=NULL){
    combination_learner_->learn(rec_dat);
  }
  for(uint ii=0; ii<learners_.size(); ii++){
    learners_[ii]->learn(rec_dat);
  }
}

