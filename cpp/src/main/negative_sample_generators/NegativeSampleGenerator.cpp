#include "NegativeSampleGenerator.h"


void NegativeSampleGenerator::update(RecDat* rec_dat){
  get_implicit_train_data(rec_dat);
  for(uint ui = 0; ui<updaters_.size(); ui++) {
    updaters_[ui]->message(UpdaterMessage::start_of_implicit_update_cycle);
  }
  for(auto sample:implicit_train_data_){
    for(auto updater:updaters_){
      updater->update(&sample);
    }
  }
  for(uint ui = 0; ui<updaters_.size(); ui++) {
    updaters_[ui]->message(UpdaterMessage::end_of_implicit_update_cycle);
  }
}

vector<RecDat>*  NegativeSampleGenerator::get_implicit_train_data(RecDat* positive_sample){
  generate(positive_sample);
  implicit_train_data_.clear();
  implicit_train_data_.push_back(*positive_sample);
  for(uint ii=0; ii<samples.size(); ii++){
    RecDat negative_sample = *positive_sample;
    negative_sample.item = samples[ii];
    negative_sample.score = 0;
    implicit_train_data_.push_back(negative_sample);
  }
  return &implicit_train_data_;
}


