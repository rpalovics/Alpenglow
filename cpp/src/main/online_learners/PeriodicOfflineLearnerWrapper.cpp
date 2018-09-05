#include "PeriodicOfflineLearnerWrapper.h"

void PeriodicOfflineLearnerWrapper::update(RecDat* rec_dat){
  period_computer_->update(rec_dat);
  if(period_computer_->end_of_period()){
    clear_model(rec_dat);
    read_model(rec_dat);
    update_model(rec_dat);
    write_model(rec_dat);
  }
}
void PeriodicOfflineLearnerWrapper::update_model(RecDat* rec_dat){
  if(learn_ and offline_learners_.size()!=0){
    RecommenderData* recommender_data =
         data_generator_->generate_recommender_data(rec_dat);
    for(auto offline_learner:offline_learners_){
      offline_learner->fit(recommender_data);
    }
  }
}

void PeriodicOfflineLearnerWrapper::clear_model(RecDat* rec_dat){
  if (clear_model_) {
    model_->clear();
  }
}
void PeriodicOfflineLearnerWrapper::read_model(RecDat* rec_dat){
  if (read_model_) { 
    int period_num = period_computer_->get_period_num();
    string file_name = base_in_file_name_ + "_" + to_string(period_num);
    ifstream ifs(file_name.c_str(), ios::binary);
    if(ifs.fail()){
      cerr << "Unable to open input file '" << file_name << "' for model " << period_num << endl;
      return;
    }
    model_->read(ifs);
    ifs.close();
  }
}
void PeriodicOfflineLearnerWrapper::write_model(RecDat* rec_dat){
  if (write_model_){ 
    int period_num = period_computer_->get_period_num();
    string file_name = base_out_file_name_ + "_" + to_string(period_num);
    ofstream ofs(file_name.c_str(), ios::binary);
    if(ofs.fail()){
      cerr << "Unable to open output file '" << file_name << "' for model " << period_num << endl;
      return;
    }
    model_->write(ofs);
    ofs.close();
  }
}
