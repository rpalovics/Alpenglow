#ifndef TIMEFRAME_DATA_GENERATOR
#define TIMEFRAME_DATA_GENERATOR

#include "DataGenerator.h"
#include "../general_interfaces/INeedExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"

struct TimeframeDataGeneratorParameters {
  int time_frame_length = 86400;
};
class TimeframeDataGenerator : public DataGenerator, public INeedExperimentEnvironment, public Initializable {
  public:
    TimeframeDataGenerator(TimeframeDataGeneratorParameters* params){
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
    ExperimentEnvironment* experiment_environment_ = NULL;
    RecommenderDataIterator* recommender_data_iterator_ = NULL;
    RecommenderData local_recommender_data_;
};

#endif
