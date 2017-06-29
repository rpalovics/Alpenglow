#include "OnlineExperiment.h"

void OnlineExperiment::run() {
  cerr << "run..." <<endl;
  while (recommender_data_iterator_->has_next()) {
    RecDat* rec_dat = recommender_data_iterator_->next();
    for (uint i = 0; i < loggers_.size(); i++) {
      loggers_[i]->run(rec_dat);
    }
    experiment_environment_.update(rec_dat);
    recommender_->learn(rec_dat);
  }
  for (uint i = 0; i < end_loggers_.size(); i++) {
    end_loggers_[i]->run(NULL);
  }
}
  
