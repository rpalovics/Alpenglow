#ifndef HIGH_PREDICTION_NEGATIVE_SAMPLE_GENERATOR_H
#define HIGH_PREDICTION_NEGATIVE_SAMPLE_GENERATOR_H

#include "NegativeSampleGenerator.h"
#include "../utils/SortPairDescendingBySecond.h"
struct HighPredictionNegativeSampleGeneratorParameters {
  int negative_rate;
  int full_negative_rate;
  int seed = 744478;
};
class HighPredictionNegativeSampleGenerator : public NegativeSampleGenerator {
  public:
    HighPredictionNegativeSampleGenerator(HighPredictionNegativeSampleGeneratorParameters* params){
      negative_rate_=params->negative_rate;
      full_negative_rate_=params->full_negative_rate;
      random_.set(params->seed);
    }
    bool self_test(){
      bool ok=NegativeSampleGenerator::self_test() && random_.self_test();
      if(negative_rate_==-1){
        ok=false;
        cerr << "error: HighPredictionNegativeSampleGenerator::negative_rate==-1" << endl;
      }
      if(full_negative_rate_==-1){
        ok=false;
        cerr << "error: HighPredictionNegativeSampleGenerator::full_negative_rate==-1" << endl;
      }
      if(negative_rate_>full_negative_rate_){
        ok=false;
        cerr << "error: HighPredictionNegativeSampleGenerator::negative_rate<full_negative_rate" << endl;
      }
      if(model_==NULL){
        ok=false;
        cerr << "error: HighPredictionNegativeSampleGenerator::model is not set." << endl;
      }
      if(train_matrix_==NULL){
        ok=false;
        cerr << "error: HighPredictionNegativeSampleGenerator::train_matrix is not set." << endl;
      }
      if(items_==NULL){
        ok=false;
        cerr << "error: HighPredictionNegativeSampleGenerator::items_ is not set." << endl;
      }
      return ok;
    }
    void set_model(Model* model){
      model_=model;
    }
    void set_train_matrix(SpMatrix* train_matrix){
      train_matrix_=train_matrix;
    }
    void set_items(vector<int>* items){
      items_=items;
    }
    vector<int>* generate(RecDat* rec_dat);
  private:
    //config parameters
    int negative_rate_;
    int full_negative_rate_;
    Random random_;
    Model* model_ = NULL;
    SpMatrix* train_matrix_ = NULL;
    vector<int>* items_ = NULL;
    //variables
    void generate_all();
    void choose_best();
    RecDat positive_sample_;
    int user_;
    int item_;
    vector<pair<int,double> > local_samples_;
};



#endif /* HIGH_PREDICTION_NEGATIVE_SAMPLE_GENERATOR_H */
