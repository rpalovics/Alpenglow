#include "OfflineExperiment.h"

void OfflineExperiment::run() {
  for(int i=0;i<offline_learners_.size();i++){
    offline_learners_[i]->iterate();
  }
  for(int i=0;i<offline_evaluators_.size();i++){
    offline_evaluators_[i]->evaluate();
  }
}
