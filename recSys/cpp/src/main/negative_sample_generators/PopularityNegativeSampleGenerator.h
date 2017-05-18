#ifndef POPULARITY_NEGATIVE_SAMPLE_GENERATOR
#define POPULARITY_NEGATIVE_SAMPLE_GENERATOR

#include "NegativeSampleGenerator.h"
#include "../utils/PopContainers.h"
#include "../recommender_data/RecommenderDataIterator.h"

struct PopularityNegativeSampleGeneratorParameters{
  int negativeRate;
  int seed;
};

class PopularityNegativeSampleGenerator: public NegativeSampleGenerator{
  public:
    PopularityNegativeSampleGenerator(PopularityNegativeSampleGeneratorParameters* params){
      negativeRate = params->negativeRate; 
      random.set(params->seed);
    }
    vector<int>* generate(RecDat* rec_dat);
    void setPopContainer(PopContainer* pop_container_){pop_container = pop_container_;}
    void set_train_matrix(SpMatrix* train_matrix_){train_matrix=train_matrix_;}
    void setRecommenderDataIterator(RecommenderDataIterator* recommender_data_iterator_){recommender_data_iterator=recommender_data_iterator_;}
    void set_items(vector<int>* items_){items=items_;}
  private:
    PopContainer *pop_container;
    vector<int>* items;
    SpMatrix* train_matrix;
    RecommenderDataIterator* recommender_data_iterator;
    Random random;
    int negativeRate;
};


#endif
