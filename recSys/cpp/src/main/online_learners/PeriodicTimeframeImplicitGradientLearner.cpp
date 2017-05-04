#include "PeriodicTimeframeImplicitGradientLearner.h"

vector<RecDat*> PeriodicTimeframeImplicitGradientLearner::generate_train_data(RecDat* rec_dat){
  cerr << "Period num=" << period_num_ << endl;
  int timeframe_begin=start_time_+period_num_*period_length_-timeframe_length_;
  vector<RecDat*> train_data;
  for(int counter = recommender_data_->get_counter();counter>=0;counter--){
    RecDat* rec_dat = recommender_data_->get(counter);
    if(rec_dat->time<=timeframe_begin) break;
    train_data.push_back(rec_dat);
  }
  return train_data;
}

void PeriodicTimeframeImplicitGradientLearner::initialize_model(RecDat* rec_dat, vector<RecDat*> train_data){
  if(local_items_!=NULL and local_train_matrix_!=NULL){
    //local items, local_train_matrix: compute items, spmatrix for timeframe
    local_train_matrix_->clear(); 
    vector<int> item_map;
    local_items_->clear();
    for(uint i=0;i<train_data.size();i++){
      RecDat* rec_dat = train_data[i];
      local_train_matrix_->insert(rec_dat->user,rec_dat->item,rec_dat->score);
      int item = rec_dat->item;
      if((int)item_map.size()<=item) item_map.resize(item+1,0);
      if(item_map[item]==0){
        item_map[item]=1;
        local_items_->push_back(item);
      }
    }
  }
  //initialize model
  if(clear_model_ and local_items_!=NULL and local_train_matrix_!=NULL){
    for(uint i=0; i<train_data.size(); i++){
      model_->add(train_data[i]);
    }
  } else {
    for(;next_add_counter_<=recommender_data_->get_counter(); next_add_counter_++){
      model_->add(recommender_data_->get(next_add_counter_));
    }
  }
}

