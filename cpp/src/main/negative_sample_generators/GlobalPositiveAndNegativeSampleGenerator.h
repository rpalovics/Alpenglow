#ifndef GLOBAL_POSITIVE_AND_NEGATIVE_SAMPLE_GENERATOR_H
#define GLOBAL_POSITIVE_AND_NEGATIVE_SAMPLE_GENERATOR_H

#include <gtest/gtest_prod.h>
#include "../utils/Random.h"
#include "NegativeSampleGenerator.h"

struct GlobalPositiveAndNegativeSampleGeneratorParameters { 
    double positive_rate,negative_rate,decay;
    int seed = 745578; 
    int threshold;
    string decay_type;
};

class GlobalPositiveAndNegativeSampleGenerator : public NegativeSampleGenerator {
  public: 
    GlobalPositiveAndNegativeSampleGenerator(GlobalPositiveAndNegativeSampleGeneratorParameters* parameters){
      set_parameters(parameters);
    };
    void set_parameters (GlobalPositiveAndNegativeSampleGeneratorParameters * parameters);
    void set_train_matrix(SpMatrix* train_matrix_){train_matrix=train_matrix_;}
    void set_items(vector<int>* items_){items=items_;}
    vector<int>* generate(RecDat* rec_dat);
    void generate_positive(RecDat* rec_dat);
    vector<RecDat>*  get_implicit_train_data(RecDat* positive_sample);
    void update(RecDat* rec_dat);
    bool self_test(){
      return NegativeSampleGenerator::self_test() && random_.self_test();
    }
  protected:
    int select(int user);
    vector<int>* items;
    vector<int> positive_relevances;
    vector<RecDat*> positive_samples;
    SpMatrix* train_matrix;
    vector<RecDat*> history;
    double positive_rate,negative_rate;
    double decay; 
    int seed_;
    Random random_;
    int threshold;
    string decay_type;
};

#endif /* GLOBAL_POSITIVE_AND_NEGATIVE_SAMPLE_GENERATOR_H */
