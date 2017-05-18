#include "OnlineExperiment.h"

//void OnlineExperiment::set_parameters(OnlineExperimentParameters * parameters){
//  min_time_ = parameters->min_time;
//  max_time_ = parameters->max_time;
//}

void OnlineExperiment::run() {
  cerr << "run..." <<endl;
  while (recommender_data_iterator_->has_next()) {
    RecDat* rec_dat = recommender_data_iterator_->next();
    //if (rec_dat->time >= max_time_ && max_time_ > 0) return;
    //if (rec_dat->time >= min_time_ && rec_dat->eval==1){
    //  recommender_->logParameters(rec_dat);
    //}
    
    for (uint i = 0; i < loggers_.size(); i++) {
      loggers_[i]->run(rec_dat);
    }
    recommender_->learn(rec_dat);
    online_data_updater_->update(rec_dat);
  }
  for (uint i = 0; i < end_loggers_.size(); i++) {
    end_loggers_[i]->run(NULL);
  }

  for(auto i : loggers_){
    //so the destructors run
    delete i;
  }
}
  
