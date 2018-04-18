#include "PredictionCreator.h"

vector<RecDat>* PredictionCreatorGlobal::run(RecDat* rec_dat){
  filter_->run(rec_dat);
  vector<pair<int,double>>* sorted_users = filter_->get_global_users();
  vector<pair<int,double>>* sorted_items = filter_->get_global_items();
  RecDat fake_rec_dat = *rec_dat;
  
  process_rectangle(sorted_users, sorted_items, 0,0,initial_threshold_,initial_threshold_, &fake_rec_dat);
  process_rectangle(sorted_users, sorted_items, initial_threshold_,0,UINT_MAX,initial_threshold_, &fake_rec_dat);
  process_rectangle(sorted_users, sorted_items, 0,initial_threshold_,initial_threshold_,UINT_MAX, &fake_rec_dat);
  process_rectangle(sorted_users, sorted_items, initial_threshold_,initial_threshold_,UINT_MAX,UINT_MAX, &fake_rec_dat);

  top_predictions_.resize(min_heap_->size());
  for(int ii=(int)min_heap_->size()-1; ii>=0; ii--){
    top_predictions_[ii]=min_heap_->get_min();
    min_heap_->delete_min();
  }
  return &top_predictions_;
}

void PredictionCreatorGlobal::process_rectangle(vector<pair<int,double> >* sorted_users, vector<pair<int,double> >* sorted_items, uint begin_user_index, uint begin_item_index, uint end_user_index, uint end_item_index, RecDat* fake_rec_dat){
  uint user_index = begin_user_index;
  uint item_index = begin_item_index;
  while(user_index<sorted_users->size() and item_index<sorted_items->size() and user_index<end_user_index and item_index<end_item_index){
    double user_bound = (*sorted_users)[user_index].second;
    double item_bound = (*sorted_items)[item_index].second;
    if(min_heap_->size()==top_k_ and (user_bound<min_heap_->get_min().score or item_bound<min_heap_->get_min().score)){break;}
    if(user_bound>item_bound){
      int user = (*sorted_users)[user_index].first;
      fake_rec_dat->user = user;
      process_line(sorted_items,item_index,end_item_index,&(fake_rec_dat->item),fake_rec_dat);
      user_index++;
    } else {
      int item = (*sorted_items)[item_index].first;
      fake_rec_dat->item = item;
      process_line(sorted_users,user_index,end_user_index,&(fake_rec_dat->user),fake_rec_dat);
      item_index++;
    }
  }
}
void PredictionCreatorGlobal::process_line(vector<pair<int,double> >* sorted_as,uint begin_a_index, uint end_a_index, int* fake_rec_dat_a, RecDat* fake_rec_dat){
  for(uint a_index=begin_a_index;a_index<min(end_a_index,(uint)sorted_as->size());a_index++){
    double a_bound = (*sorted_as)[a_index].second;
    if(min_heap_->size()==top_k_ and a_bound<=min_heap_->get_min().score){break;}
    (*fake_rec_dat_a)=(*sorted_as)[a_index].first;
    if(exclude_known_!=1 or train_matrix_->get(fake_rec_dat->user, fake_rec_dat->item)==0){
      fake_rec_dat->score=model_->prediction(fake_rec_dat);
      min_heap_->insert(*fake_rec_dat);
    }
  }
}

vector<RecDat>* PredictionCreatorPersonalized::run(RecDat* rec_dat){
  if(ranking_model_ != NULL && filter_ == NULL){
    return run_ranking_model(rec_dat);
  } else {
    return run_bruteforce(rec_dat);
  }
}

vector<RecDat>* PredictionCreatorPersonalized::run_bruteforce(RecDat* rec_dat){ //TODO test
  filter_->run(rec_dat);
  RecDat fake_rec_dat = *rec_dat; //TODO lehet NULL
  vector<pair<int,double>>* sorted_items = filter_->get_personalized_items(rec_dat->user);
  for(auto item_bound: *sorted_items){
    if(min_heap_->size()==top_k_ and item_bound.second!=-1 and item_bound.second<min_heap_->get_min().score){ break; } //all consecutive items have lower prediction
    if(exclude_known_==1 and train_matrix_->get(rec_dat->user, item_bound.first)!=0){ continue; } //invalid item, not new for the user
    fake_rec_dat.item = item_bound.first;
    fake_rec_dat.score = model_->prediction(&fake_rec_dat);
    min_heap_->insert(fake_rec_dat);
  }

  top_predictions_.resize(min_heap_->size()); //TODO ez legyen a heapben megirva
  for(int ii=(int)min_heap_->size()-1; ii>=0; ii--){
    top_predictions_[ii]=min_heap_->get_min();
    min_heap_->delete_min();
  }
  return &top_predictions_;
}

vector<RecDat>* PredictionCreatorPersonalized::run_ranking_model(RecDat* rec_dat){
  SpMatrix *matrix = train_matrix_;
  if(!exclude_known_){
    matrix = &dummy_train_matrix_;
  }
  vector<pair<int,double>> predictions = ranking_model_->get_top_list(rec_dat->user, top_k_, matrix);
  top_predictions_.clear();
  for(auto &pair : predictions){
    int id;
    double score;
    tie(id,score)=pair;
    RecDat rd;
    rd.user=rec_dat->user;
    rd.item=id;
    rd.score=score;
    top_predictions_.push_back(rd);
  }
  return &top_predictions_;
}