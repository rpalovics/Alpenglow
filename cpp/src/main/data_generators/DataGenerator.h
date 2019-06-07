#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"

class DataGenerator {
  public:
    virtual RecommenderData* generate_recommender_data(RecDat*)=0;
    bool self_test(){ return true; }
};

#endif /* DATA_GENERATOR_H */
