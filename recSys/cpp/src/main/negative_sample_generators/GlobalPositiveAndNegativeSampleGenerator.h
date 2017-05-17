#ifndef GLOBAL_NEGATIVE_AND_POSITIVE_SAMPLE_GENERATOR
#define GLOBAL_NEGATIVE_AND_POSITIVEsSAMPLE_GENERATOR

#include <gtest/gtest_prod.h>
#include "../utils/Random.h"
#include "NegativeSampleGenerator.h"

struct GlobalPositiveAndNegativeSampleGeneratorParameters{ 
    double positiveRate,negativeRate,decay;
    bool initializeAll;
    int maxItem;
    int seed; 
    int threshold;
    string decay_type;
};

class GlobalPositiveAndNegativeSampleGenerator : public NegativeSampleGenerator {
  public: 
    GlobalPositiveAndNegativeSampleGenerator(GlobalPositiveAndNegativeSampleGeneratorParameters * parameters){
      setParameters(parameters);
    };
    void setParameters (GlobalPositiveAndNegativeSampleGeneratorParameters * parameters);
    void setTrainMatrix(SpMatrix* train_matrix_){train_matrix=train_matrix_;}
    void set_items(vector<int>* items_){ if(!initializeAll) items=items_;}
    vector <int> * generate(RecDat * recDat);
    void generate_positive(RecDat * recDat);
    vector<RecDat>*  get_implicit_train_data(RecDat* positive_sample);
    void update(RecDat* rec_dat);
  protected:
    int select(int user);
    vector <int> * items;
    vector <int> positive_relevances;
    vector <RecDat *> positive_samples;
    SpMatrix * train_matrix;
    vector<RecDat *> history;
    double positiveRate,negativeRate;
    bool initializeAll;
    int maxItem;
    double decay; 
    int seed_;
    Random random_;
    int threshold;
    string decay_type;
};

#endif
