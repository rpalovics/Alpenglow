#include "PeriodicLearner.h"

void PeriodicLearner::init(){
  cerr << "PeriodicLearner::init is called." << endl;
  clear_model(NULL);
  read_model(NULL);
  period_num_=1;
}

void PeriodicLearner::learn(RecDat* rec_dat){
  int next_period_num=compute_next_period_num(rec_dat);
  if(next_period_num!=period_num_){
    clear_model(rec_dat);
    read_model(rec_dat);
    update_model(rec_dat);
    write_model(rec_dat);
    period_num_ = next_period_num;
  }
}

int PeriodicLearner::compute_next_period_num(RecDat* rec_dat){
  if (mode_=="time-based"){
    if(start_time_==-1){ start_time_=rec_dat->time; }
    int next_time = recommender_data_->get_following_timestamp(); 
    if(next_time!=-1){
      int next_period_num = (next_time-start_time_)/period_length_+1;
      return next_period_num;
    } else { //there is a virtual endperiod-point at the end of the data
      if(end_period_num_ == -1){
        end_period_num_ = period_num_+1;
      }
      return end_period_num_;
    }
  }
  if (mode_=="samplenum-based"){
    int next_sample_index = recommender_data_->get_counter()+1;
    if (next_sample_index==recommender_data_->size()) {
      return period_num_+1;
    } else {
      int next_period_num=next_sample_index/samplenum_in_periods_+1;
      return next_period_num;
    }
  }
  throw 1;
}

void PeriodicLearner::clear_model(RecDat* rec_dat){
  if (clear_model_) {
    model_->clear();
  }
}
void PeriodicLearner::read_model(RecDat* rec_dat){
  if (read_model_) { 
    string file_name = base_in_file_name_ + "_" + to_string(period_num_);
    ifstream ifs(file_name.c_str(), ios::binary);
    if(ifs.fail()){
      cerr << "Unable to open file for model " << period_num_ << endl;
      return;
    }
    model_->read(ifs);
    ifs.close();
  }
}
void PeriodicLearner::write_model(RecDat* rec_dat){
  if (write_model_){ 
    string file_name = base_out_file_name_ + "_" + to_string(period_num_);
    ofstream ofs(file_name.c_str(), ios::binary);
    model_->write(ofs);
    ofs.close();
  }
}
