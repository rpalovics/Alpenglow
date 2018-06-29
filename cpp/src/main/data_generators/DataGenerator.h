#ifndef DATA_GENERATOR
#define DATA_GENERATOR

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"

class DataGenerator {
  public:
    virtual RecommenderData* generate_recommender_data(RecDat*)=0;
};

#endif
