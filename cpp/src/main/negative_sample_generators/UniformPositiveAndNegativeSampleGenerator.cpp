#include "UniformPositiveAndNegativeSampleGenerator.h"

void UniformPositiveAndNegativeSampleGenerator::set_parameters(UniformPositiveAndNegativeSampleGeneratorParameters * parameters){
  items = NULL;
  train_matrix = NULL;
  positive_rate = parameters->positive_rate;
  negative_rate = parameters->negative_rate;
  decay = parameters->decay;
  generate_user_ = parameters->generate_user;
  generate_item_ = parameters->generate_item;
  sample_ = parameters->sample;
  sample_num_type_ = parameters->sample_num_type;
  distribution_ = parameters->distribution;
  if(distribution_==""){
    distribution_ = "uniform";
    cerr << "UniformPositiveAndNegativeSampleGeneratorParameters::distribution is not set, default is uniform." << endl;
  }
  seed_ = parameters->seed;
  if(seed_==-1){
    cerr<<"UniformPositiveAndNegativeSampleGeneratorParameters::seed is not set, default seed is 14580124." << endl;
    seed_ = 14580124;
  }
  random_.set(seed_);
}


vector <int> * UniformPositiveAndNegativeSampleGenerator::generate_positive(RecDat * rec_dat, string type){
  vector <int> * history = NULL;
  if(type == "item" and (int)item_histories_.size() > rec_dat->item and item_histories_[rec_dat->item]!= NULL)
    history = item_histories_[rec_dat->item];
  if(type == "user" and (int)user_histories_.size() > rec_dat->user and user_histories_[rec_dat->user]!= NULL)
    history = user_histories_[rec_dat->user];
  if(history != NULL){
    int history_size = history->size();
    if(sample_){
      double p = get_positive_rate(history_size);
      int num = (int) p;
      if(random_.get() < p-(int)p) num++;
      for(int ii=0; ii < num; ii++){ 
        int index = 0;
        if(distribution_ == "uniform")
          index = random_.get(history_size);
        else if (distribution_ == "linear")
          index = random_.get_linear(history_size);
        else if (distribution_ == "neg_linear")
          index = history_size - random_.get_linear(history_size) - 1;
        else if (distribution_ == "geometric")
          index = history_size - random_.get_geometric(decay,history_size);
        if(index >= 0){
          if (type == "user" ) {
            user_positive_samples.push_back(history->at(index));
            user_positive_relevances.push_back(history_size - 1 - index);
          }
          else{
            item_positive_samples.push_back(history->at(index));
            item_positive_relevances.push_back(history_size - 1 - index);
          }
        }
      }
    }
    else{
      for(int ii=0; ii<history_size; ii++){
       if (type == "user") {
         user_positive_samples.push_back(history->at(ii));
         user_positive_relevances.push_back(history_size - 1 - ii);
       }
       else{
         item_positive_samples.push_back(history->at(ii));
         item_positive_relevances.push_back(history_size - 1 - ii); 
      }
      }
    }
  if(type == "user") return &user_positive_samples;
  else return &item_positive_samples;
  } 
  /* else (history==NULL) */ throw logic_error("User history is NULL."); //never happens
}


vector <int> * UniformPositiveAndNegativeSampleGenerator::generate(RecDat * rec_dat){
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


vector<RecDat>*  UniformPositiveAndNegativeSampleGenerator::get_implicit_train_data(RecDat* positive_sample){ 
  item_positive_samples.clear();
  item_positive_relevances.clear();
  user_positive_samples.clear();
  user_positive_relevances.clear();
  generate(positive_sample);
  if(generate_user_) generate_positive(positive_sample,"user");
  if(generate_item_) generate_positive(positive_sample,"item");
  update(positive_sample);
  implicit_train_data_.clear();
  for(uint ii=0; ii<samples.size(); ii++){
    RecDat negative_sample = *positive_sample;
    negative_sample.item = samples[ii];
    negative_sample.score = 0;
    negative_sample.eval = 1;
    implicit_train_data_.push_back(negative_sample);
  }
  for(uint ii=0; ii<user_positive_samples.size(); ii++){
    RecDat pos_sample = *positive_sample;
    pos_sample.item = user_positive_samples[ii];
    pos_sample.score = 1;
    pos_sample.eval = 2 + user_positive_relevances[ii]; 
    implicit_train_data_.push_back(pos_sample);
  }
  for(uint ii=0; ii<item_positive_samples.size(); ii++){
    RecDat pos_sample = *positive_sample;
    pos_sample.user = item_positive_samples[ii];
    pos_sample.score = 1;
    pos_sample.eval = 2 + item_positive_relevances[ii]; 
    implicit_train_data_.push_back(pos_sample);
  }

  implicit_train_data_.push_back(*positive_sample);
  return &implicit_train_data_;
}

void UniformPositiveAndNegativeSampleGenerator::update(RecDat* rec_dat){
  int user = rec_dat->user;
  int item = rec_dat->item;
  if((int)item_histories_.size()<=item){ item_histories_.resize(item+1); }
  if(item_histories_[item]==NULL){ item_histories_[item] = new vector<int>; }
  item_histories_[item]->push_back(user);
  if((int)user_histories_.size()<=user){ user_histories_.resize(user+1); }
  if(user_histories_[user]==NULL){ user_histories_[user] = new vector<int>; }
  user_histories_[user]->push_back(item);

}
