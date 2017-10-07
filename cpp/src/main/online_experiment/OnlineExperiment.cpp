#include "OnlineExperiment.h"

OnlineExperiment::OnlineExperiment(OnlineExperimentParameters* parameters){
  srand(parameters->random_seed);
  experiment_environment_ = new ExperimentEnvironment();
  experiment_environment_->set_parameters(parameters);
}

OnlineExperiment::~OnlineExperiment(){
  delete experiment_environment_;
}

void OnlineExperiment::set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){
  recommender_data_iterator_ = recommender_data_iterator;
  experiment_environment_->set_recommender_data_iterator(recommender_data_iterator);
}

void OnlineExperiment::inject_experiment_environment_into(NeedsExperimentEnvironment *object){
  object->set_experiment_environment(experiment_environment_);
}

void OnlineExperiment::run() {
  // cerr << "run..." <<endl;
  while (recommender_data_iterator_->has_next()) {
    RecDat* rec_dat = recommender_data_iterator_->next();
    for (auto logger : loggers_) {
      logger->run(rec_dat);
    }
    experiment_environment_->update(rec_dat);
    for (auto updater : updaters_){
      updater->update(rec_dat);
    }
  }
  for (auto end_logger : end_loggers_) {
    end_logger->run(NULL);
  }
}
  
