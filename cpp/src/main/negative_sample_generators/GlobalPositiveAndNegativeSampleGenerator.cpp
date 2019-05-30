#include "GlobalPositiveAndNegativeSampleGenerator.h"

void GlobalPositiveAndNegativeSampleGenerator::set_parameters(GlobalPositiveAndNegativeSampleGeneratorParameters * parameters){
  items = NULL;
  train_matrix = NULL;
  positive_rate = parameters->positive_rate;
  negative_rate = parameters->negative_rate;
  decay = parameters->decay;
  threshold = parameters->threshold;
  decay_type = parameters->decay_type;
  if(decay_type=="") decay_type = "uniform"; 
  seed_ = parameters->seed;
  if(seed_==-1) seed_ = 14580124;
  random_.set(seed_);
}


void GlobalPositiveAndNegativeSampleGenerator::generate_positive(RecDat * rec_dat){
  int history_size = history.size();
  if(history_size>0){
    double p = positive_rate;
    int num = (int) p;
    if(random_.get() < p-(int)p) num++;
    for(int ii=0; ii < num; ii++){ 
      int index = 0;
      if(decay_type == "uniform") index = random_.get(history_size);
      else if (decay_type == "geometric") index = history_size - random_.get_geometric(decay,history_size);
      if(index >= 0){ 
          positive_samples.push_back(history[index]);
          positive_relevances.push_back(history_size - 1 - index);
      }
    }
  }
}


vector<int>* GlobalPositiveAndNegativeSampleGenerator::generate(RecDat * rec_dat){
  int learnt = 0;
  samples.clear();
  int user_activity = train_matrix->row_size(rec_dat->user);
  while(learnt < negative_rate && learnt<(int)items->size()-user_activity-1){
    int item = items->at(random_.get(items->size()));
    if(!train_matrix->has_value(rec_dat->user,item) && item!=rec_dat->item){ 
      samples.push_back(item);
      learnt++;    
    }
  }  
  return &samples;
}


vector<RecDat>*  GlobalPositiveAndNegativeSampleGenerator::get_implicit_train_data(RecDat* positive_sample){  
  positive_samples.clear();
  positive_relevances.clear(); 
  generate(positive_sample);
  generate_positive(positive_sample);
  update(positive_sample);
  implicit_train_data_.clear();
  for(uint ii=0; ii<samples.size(); ii++){
    RecDat negative_sample = *positive_sample;
    negative_sample.item = samples[ii];
    negative_sample.score = 0;
    negative_sample.eval = 1;
    implicit_train_data_.push_back(negative_sample);
  }
  for(uint ii=0; ii<positive_samples.size(); ii++){
    RecDat pos_sample = *positive_sample;
    pos_sample.item = positive_samples[ii]->item;
    pos_sample.user = positive_samples[ii]->user;
    pos_sample.score = 1;
    pos_sample.eval = 2 +  positive_relevances[ii]; 
    implicit_train_data_.push_back(pos_sample);
  }
  implicit_train_data_.push_back(*positive_sample);
  return &implicit_train_data_;
}

void GlobalPositiveAndNegativeSampleGenerator::update(RecDat* rec_dat){
  history.push_back(rec_dat);
}
