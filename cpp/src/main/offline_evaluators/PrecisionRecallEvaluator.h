#ifndef PRECISION_RECALL_EVALUATOR
#define PRECISION_RECALL_EVALUATOR

//SIP_AUTOCONVERT

#include <string>
#include <gtest/gtest_prod.h>
#include "../models/Model.h"
#include "../filters/ModelFilter.h"
#include "../utils/MinHeap.h"
#include "OfflineEvaluator.h"

using namespace std;

struct PrecisionRecallEvaluatorParameters{
  int cutoff;
  string test_file_name;
  string test_file_type;
  int time;
};

class PrecisionRecallEvaluator : public OfflineEvaluator{
  public:
    PrecisionRecallEvaluator(PrecisionRecallEvaluatorParameters* params){
      cutoff_ = params->cutoff;
      test_data_.read_from_file(params->test_file_name, params->test_file_type);
      time_ = params->time;
      model_ = NULL;
      train_data_ = NULL;
      model_filter_ = NULL;
    }
    virtual ~PrecisionRecallEvaluator(){
    }
    void set_model(Model* model){ model_ = model; }
    void set_train_data(RecommenderData* recommender_data){ train_data_ = recommender_data; }
    void set_model_filter(ModelFilter* model_filter){ model_filter_ = model_filter; }
    virtual void evaluate(){
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

};
    bool self_test(){
      bool OK = OfflineEvaluator::self_test();
      if (cutoff_ < 0){
        OK = false;
        cerr << "PrecisionRecallEvaluator::cutoff_ < -1" << endl;
      }
      if(model_ == NULL){
        OK = false;
        cerr << "PrecisionRecallEvaluator::model is not set." << endl;
      }
      if(train_data_ == NULL){
        OK = false;
        cerr << "PrecisionRecallEvaluator::train_data is not set." << endl;
      }
      if(test_data_.size() == 0){
        OK = false;
        cerr << "PrecisionRecallEvaluator::test_data size is zero." << endl;
      }
      if(model_filter_ == NULL){
        OK = false;
        cerr << "PrecisionRecallEvaluator::model_filter is not set." << endl;
      }
      return OK;
    }
  private:
    int cutoff_;
    int time_;
    Model* model_;
    LegacyRecommenderData test_data_;
    RecommenderData* train_data_;
    ModelFilter* model_filter_;
    FRIEND_TEST(TestPrecisionRecallEvaluator, general);
    vector<pair<int, double> >* train_items_;
    int compute_true_positive(int user){
  RecDat rec_dat;
  rec_dat.user = user;
  rec_dat.time = time_;
  MinHeap top_list(cutoff_);
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
  map<int, double>* positive = test_data_.get_full_matrix()->get(user);
  for(;top_list.size()>0;top_list.delete_min()){
    rec_dat = top_list.get_min();
    map<int, double>::iterator it = positive->find(rec_dat.item);
    if(it!=positive->end()) true_positive++;
  } 
  return true_positive;
};
};


#endif
