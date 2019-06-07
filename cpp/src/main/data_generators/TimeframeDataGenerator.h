#ifndef TIMEFRAME_DATA_GENERATOR_H
#define TIMEFRAME_DATA_GENERATOR_H

//SIP_AUTOCONVERT

#include "DataGenerator.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"

struct TimeframeDataGeneratorParameters {
  int timeframe_length = 86400;
};
class TimeframeDataGenerator : public DataGenerator, public NeedsExperimentEnvironment, public Initializable {
  public:
    TimeframeDataGenerator(TimeframeDataGeneratorParameters* params){
      timeframe_length_ = params->timeframe_length;
    }
    RecommenderData* generate_recommender_data(RecDat*) override;
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){
     recommender_data_iterator_ = recommender_data_iterator;
    }
    bool self_test(){
      bool ok=DataGenerator::self_test();
      if(recommender_data_iterator_==NULL) ok=false;
      return ok;
    }
  protected:
    bool autocalled_initialize() override {
      if(recommender_data_iterator_==NULL){
        if (experiment_environment_==NULL) return false;
        if (timeframe_length_<0) return false;
        recommender_data_iterator_=experiment_environment_->get_recommender_data_iterator();
      }
      return true;
    }
  private:
    int timeframe_length_;
    const RecommenderDataIterator* recommender_data_iterator_ = NULL;
    RecommenderData local_recommender_data_;
};

#endif /* TIMEFRAME_DATA_GENERATOR_H */
