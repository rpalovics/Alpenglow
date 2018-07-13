
class DataGenerator {
  %TypeHeaderCode
  #include "data_generators/DataGenerator.h"
  %End
  public:
    virtual RecommenderData* generate_recommender_data(RecDat*)=0;
};

