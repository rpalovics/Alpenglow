#ifndef UNIFORM_NEGATIVE_AND_POSITIVE_SAMPLE_GENERATOR
#define UNIFORM_NEGATIVE_AND_POSITIVEsSAMPLE_GENERATOR

#include <gtest/gtest_prod.h>
#include "../utils/Random.h"
#include "../updaters/ModelUpdater.h"
#include "NegativeSampleGenerator.h"

struct UniformPositiveAndNegativeSampleGeneratorParameters{ 
    double positiveRate,negativeRate,decay;
    bool initializeAll;
    int maxItem;
    string distribution;
    int seed;
    bool generate_user,generate_item,sample;
    string sample_num_type;
};

class UniformPositiveAndNegativeSampleGenerator : public NegativeSampleGenerator, public ModelSimpleUpdater {
  public: 
    UniformPositiveAndNegativeSampleGenerator(UniformPositiveAndNegativeSampleGeneratorParameters * parameters){
      setParameters(parameters);
    };
    void setParameters (UniformPositiveAndNegativeSampleGeneratorParameters * parameters);
    void setTrainMatrix(SpMatrix* train_matrix_){
      train_matrix=train_matrix_;
    }
    void set_items(vector<int>* items_){
      if(!initializeAll) items=items_;
    }
    vector <int> * generate(RecDat * recDat);
    vector <int> * generate_positive(RecDat * recDat, string type);
    vector<RecDat>*  get_implicit_train_data(RecDat* positive_sample);
    void update(RecDat* rec_dat);
    bool selfTest(){
      bool ok = NegativeSampleGenerator::selfTest();
      if(positiveRate < 0){
        ok=false;
        cerr << "UniformPositiveAndNegativeSampleGeneratorParameters::positiveRate is negative." << endl;
      }
      if(initializeAll && maxItem<0){
        ok = false;
        cerr << "UniformPositiveAndNegativeSampleGeneratorParameters::maxItem is negative but initializeAll is set." << endl;
      }
      if(items==NULL){
        ok = false;
        cerr << "UniformPositiveAndNegativeSampleGeneratorParameters::items is not set." << endl;
      }
      if(train_matrix == NULL){
        ok = false;
        cerr << "UniformPositiveAndNegativeSampleGeneratorParameters::train_matrix is not set." << endl;
      }
      if(distribution_ != "uniform" and distribution_ != "linear" and distribution_ != "geometric"){
        ok = false;
        cerr << "UniformPositiveAndNegativeSampleGeneratorParameters::distribution should be \"uniform\" or \"linear\"." << endl;
      }
      return ok;
    }
  protected:
    int select(int user);
    double getPositiveRate(int history_size){
      if(sample_num_type_ == "constant") return positiveRate;
      else if(sample_num_type_ == "pow") return positiveRate / ((double) history_size);
      else return positiveRate / ((double) log(2)/log(history_size + 1));
    }
    vector <int> * items;
    vector <int> user_positive_samples, user_positive_relevances;
    vector <int> item_positive_samples, item_positive_relevances;
    SpMatrix * train_matrix;
    vector<vector<int>*> item_histories_, user_histories_;
    double positiveRate,negativeRate;
    bool initializeAll;
    int maxItem;
    double decay;
    string distribution_;
    int seed_;
    Random random_;
    bool generate_user_, generate_item_,sample_;
    string sample_num_type_;
};

#endif
