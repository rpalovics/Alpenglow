#include "GlobalPositiveAndNegativeSampleGenerator.h"

void GlobalPositiveAndNegativeSampleGenerator::set_parameters(GlobalPositiveAndNegativeSampleGeneratorParameters * parameters){
  items = NULL;
  train_matrix = NULL;
  positiveRate = parameters->positiveRate;
  negativeRate = parameters->negativeRate;
  initializeAll=parameters->initializeAll;
  decay = parameters->decay;
  threshold = parameters->threshold;
  if(initializeAll){
    maxItem=parameters->maxItem;
    items=new vector<int>(maxItem+1);
    for(int i=0;i<items->size();i++){items->at(i)=i;}
  }
  decay_type = parameters->decay_type;
  if(decay_type=="") decay_type = "uniform"; 
  seed_ = parameters->seed;
  if(seed_==-1) seed_ = 14580124;
  random_.set(seed_);
}


void GlobalPositiveAndNegativeSampleGenerator::generate_positive(RecDat * recDat){
  int history_size = history.size();
  if(history_size>0){
    double p = positiveRate;
    int num = (int) p;
    if(rand()/(RAND_MAX+(double)1) < p-(int)p) num++;
    for(uint ii=0; ii < num; ii++){ 
      int index = 0;
      if(decay_type == "uniform") index = random_.get(history_size);
      else if (decay_type == "geometric") index = history_size - random_.get_geometric(history_size,decay,threshold) - 1;
      if(index >= 0){ 
          positive_samples.push_back(history[index]);
          positive_relevances.push_back(history_size - 1 - index);
      }
    }
  }
}


vector <int> * GlobalPositiveAndNegativeSampleGenerator::generate(RecDat * recDat){
  int learnt = 0;
  samples.clear();
  int userActivity = train_matrix->rowSize(recDat->user);
  while(learnt < negativeRate && learnt<(int)items->size()-userActivity-1){
    int item = items->at((int)(rand()/(RAND_MAX+1.0)*(items->size())));
    if(!train_matrix->hasValue(recDat->user,item) && item!=recDat->item){ 
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
