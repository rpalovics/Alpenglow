#ifndef TIMEFRAME_DATA_GENERATOR
#define TIMEFRAME_DATA_GENERATOR

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
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override {
      experiment_environment_ = experiment_environment;
    }
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){
     recommender_data_iterator_ = recommender_data_iterator;
    }
    bool self_test(){
      bool ok=true;
      if(recommender_data_iterator_==NULL) ok=false;
      return ok;
    }
  protected:
    bool autocalled_initialize() override {
      if(recommender_data_iterator_==NULL){ recommender_data_iterator_=experiment_environment_->get_recommender_data_iterator(); }
      return true;
    }
  private:
    int timeframe_length_ = 86400;
    ExperimentEnvironment* experiment_environment_ = NULL;
    RecommenderDataIterator* recommender_data_iterator_ = NULL;
    RecommenderData local_recommender_data_;
};

#endif
