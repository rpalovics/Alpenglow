#include "NegativeSampleGenerator.h"


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


