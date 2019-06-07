#ifndef PRECISION_RECALL_EVALUATOR_H
#define PRECISION_RECALL_EVALUATOR_H

//SIP_AUTOCONVERT

#include <string>
#include <gtest/gtest_prod.h>
#include "../models/Model.h"
#include "../filters/ModelFilter.h"
#include "../utils/Toplist.h"
#include "OfflineEvaluator.h"

using namespace std;

struct PrecisionRecallEvaluatorParameters {
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
    }
    void set_model(Model* model){ model_ = model; }
    void set_train_data(RecommenderData* recommender_data){ train_data_ = recommender_data; }
    void set_model_filter(ModelFilter* model_filter){ model_filter_ = model_filter; }
    virtual void evaluate();
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
    Model* model_ = NULL;
    LegacyRecommenderData test_data_;
    RecommenderData* train_data_ = NULL;
    ModelFilter* model_filter_ = NULL;
    vector<pair<int, double>>* train_items_;
    int compute_true_positive(int user);
    FRIEND_TEST(TestPrecisionRecallEvaluator, general);
};


#endif /* PRECISION_RECALL_EVALUATOR_H */
