#ifndef UNIFORM_NEGATIVE_AND_POSITIVE_SAMPLE_GENERATOR
#define UNIFORM_NEGATIVE_AND_POSITIVE_SAMPLE_GENERATOR

#include <gtest/gtest_prod.h>
#include "../utils/Random.h"
#include "../models/ModelUpdater.h"
#include "NegativeSampleGenerator.h"

struct UniformPositiveAndNegativeSampleGeneratorParameters{ 
    double positive_rate,negative_rate,decay;
    bool initialize_all;
    int max_item;
    string distribution;
    int seed;
    bool generate_user,generate_item,sample;
    string sample_num_type;
};

class UniformPositiveAndNegativeSampleGenerator : public NegativeSampleGenerator, public Updater {
  public: 
    UniformPositiveAndNegativeSampleGenerator(UniformPositiveAndNegativeSampleGeneratorParameters * parameters){
      set_parameters(parameters);
    };
    void set_parameters (UniformPositiveAndNegativeSampleGeneratorParameters * parameters);
    void set_train_matrix(SpMatrix* train_matrix_){
      train_matrix=train_matrix_;
    }
    void set_items(vector<int>* items_){
      if(!initialize_all) items=items_;
    }
    vector <int> * generate(RecDat * rec_dat);
    vector <int> * generate_positive(RecDat * rec_dat, string type);
    vector<RecDat>*  get_implicit_train_data(RecDat* positive_sample);
    void update(RecDat* rec_dat);
    bool self_test(){
      bool ok = NegativeSampleGenerator::self_test();
      if(positive_rate < 0){
        ok=false;
        cerr << "UniformPositiveAndNegativeSampleGeneratorParameters::positive_rate is negative." << endl;
      }
      if(initialize_all && max_item<0){
        ok = false;
        cerr << "UniformPositiveAndNegativeSampleGeneratorParameters::max_item is negative but initialize_all is set." << endl;
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
    double get_positive_rate(int history_size){
      if(sample_num_type_ == "constant") return positive_rate;
      else if(sample_num_type_ == "pow") return positive_rate / ((double) history_size);
      else return positive_rate / ((double) log(2)/log(history_size + 1));
    }
    vector <int> * items;
    vector <int> user_positive_samples, user_positive_relevances;
    vector <int> item_positive_samples, item_positive_relevances;
    SpMatrix * train_matrix;
    vector<vector<int>*> item_histories_, user_histories_;
    double positive_rate,negative_rate;
    bool initialize_all;
    int max_item;
    double decay;
    string distribution_;
    int seed_;
    Random random_;
    bool generate_user_, generate_item_,sample_;
    string sample_num_type_;
};

#endif
