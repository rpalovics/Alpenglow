#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"

class DataGenerator {
  public:
    virtual RecommenderData* generate_recommender_data(RecDat*)=0;
};

#endif /* DATA_GENERATOR_H */
