#include "OnlineExperiment.h"

void OnlineExperiment::run() {
  cerr << "run..." <<endl;
  while (recommender_data_iterator_->has_next()) {
    RecDat* rec_dat = recommender_data_iterator_->next();
    for (auto logger : loggers_) {
      logger->run(rec_dat);
    }
    experiment_environment_.update(rec_dat);
    for (auto learner : learners_){
      learner->learn(rec_dat);
    }
  }
  for (auto end_logger : end_loggers_) {
    end_logger->run(NULL);
  }
}
  
