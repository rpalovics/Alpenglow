#include "SimpleLearner.h"

void SimpleLearner::learn(RecDat* rec_dat){
  model_->add(rec_dat);
  for(auto updater_it = model_simple_updaters_.begin(); updater_it!=model_simple_updaters_.end(); updater_it++){
    (*updater_it)->update(rec_dat);
  }
}
