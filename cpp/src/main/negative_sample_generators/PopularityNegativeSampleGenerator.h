#ifndef POPULARITY_NEGATIVE_SAMPLE_GENERATOR_H
#define POPULARITY_NEGATIVE_SAMPLE_GENERATOR_H

#include "NegativeSampleGenerator.h"
#include "../utils/PopContainers.h"
#include "../recommender_data/RecommenderDataIterator.h"

struct PopularityNegativeSampleGeneratorParameters {
  int negative_rate;
  int seed = 745578;
};

class PopularityNegativeSampleGenerator: public NegativeSampleGenerator{
  public:
    PopularityNegativeSampleGenerator(PopularityNegativeSampleGeneratorParameters* params){
      negative_rate = params->negative_rate; 
      random_.set(params->seed);
    }
    vector<int>* generate(RecDat* rec_dat);
    void set_pop_container(PopContainer* pop_container_){pop_container = pop_container_;}
    void set_train_matrix(SpMatrix* train_matrix_){train_matrix=train_matrix_;}
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator_){recommender_data_iterator=recommender_data_iterator_;}
    void set_items(vector<int>* items_){items=items_;}
    bool self_test(){
      return NegativeSampleGenerator::self_test() && random_.self_test();
    }
  private:
    const PopContainer *pop_container;
    vector<int>* items;
    SpMatrix* train_matrix;
    RecommenderDataIterator* recommender_data_iterator;
    Random random_;
    int negative_rate;
};


#endif /* POPULARITY_NEGATIVE_SAMPLE_GENERATOR_H */
