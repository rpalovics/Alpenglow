#include "OnlineRecommender.h"

double OnlineRecommender::prediction(RecDat* rec_dat){
  return model_->prediction(rec_dat);
}

void OnlineRecommender::learn(RecDat* rec_dat){
  //if(learner!=NULL) learner->learn(rec_dat);
  learner_->learn(rec_dat);
}
