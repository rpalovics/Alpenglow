#include "PrecisionRecallEvaluator.h"
void PrecisionRecallEvaluator::evaluate(){
  vector<int>* users = test_data_.get_all_users();
  double precision_sum = 0;
  double recall_sum = 0;
  RecDat zero_time_rd;
  zero_time_rd.time = 0;
  model_filter_->run(&zero_time_rd);
  train_items_ = model_filter_-> get_global_items();
  for(uint i=0;i<users->size();i++){
    int user = users->at(i);
    int true_positive = compute_true_positive(user);
    precision_sum += true_positive/(double)test_data_.get_full_matrix()->get(user)->size();
    recall_sum += true_positive/(double)cutoff_;
  }
  double precision_avg = precision_sum/users->size();
  double recall_avg = recall_sum/users->size();

  cout << "precision_avg=" << precision_avg << endl;
  cout << "recall_avg=" << recall_avg << endl;

}
namespace {
bool compare_rec_dat(RecDat a, RecDat b){
  return a.score > b.score;
}
} //namesapce
int PrecisionRecallEvaluator::compute_true_positive(int user){
  RecDat rec_dat;
  rec_dat.user = user;
  rec_dat.time = time_;
  Toplist<RecDat,::compare_rec_dat> top_list(cutoff_);
  for(uint i=0;i<train_items_->size();i++){
    pair<int, double> value = train_items_->at(i);
    if(top_list.size()==cutoff_ and value.second < top_list.get_min().score){
      break;
    }
    rec_dat.item = value.first;
    double prediction = model_->prediction(&rec_dat);
    rec_dat.score = prediction;
    top_list.insert(rec_dat);
  }
  int true_positive = 0;
  const map<int, double>* positive = test_data_.get_full_matrix()->get(user);
  for(;top_list.size()>0;top_list.delete_min()){
    rec_dat = top_list.get_min();
    auto it = positive->find(rec_dat.item);
    if(it!=positive->end()) true_positive++;
  } 
  return true_positive;
}
