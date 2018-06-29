#ifndef SAMPLING_DATA_GENERATOR
#define SAMPLING_DATA_GENERATOR

//SIP_AUTOCONVERT

#include "DataGenerator.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"

struct SamplingDataGeneratorParameters {
  string distribution = "uniform";
  int number_of_samples = 0;
  double y = 10; //parameter of arctg random
  double geometric_param = 0.5; //parameter of geometric random
};
class SamplingDataGenerator : public DataGenerator, public Initializable, public NeedsExperimentEnvironment {
  public:
    SamplingDataGenerator(SamplingDataGeneratorParameters* params){
      distribution_ = params->distribution;
      number_of_samples_ = params->number_of_samples;
      y_ = params->y;
      geometric_param_ = params->geometric_param;
    }
    RecommenderData* generate_recommender_data(RecDat*) override;
    void set_experiment_environment(ExperimentEnvironment* experiment_environment){
      experiment_environment_ = experiment_environment;
    }
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){
     recommender_data_iterator_ = recommender_data_iterator;
    }
    bool self_test(){
      bool ok=true;
      if(recommender_data_iterator_==NULL) ok=false;
      if(number_of_samples_<0) ok=false;
      if(geometric_param_>1) ok=false; //it's a probability value...
      if(distribution_!="uniform"
         and distribution_!="linear"
         and distribution_!="arctg"
         and distribution_!="geometric") ok=false;
      if(random_==NULL) ok=false;
      return ok;
    }
  protected:
    bool autocalled_initialize() override {
      if(recommender_data_iterator_==NULL){ recommender_data_iterator_=experiment_environment_->get_recommender_data_iterator(); }
      random_ = experiment_environment_->get_random();
      return true;
    }
  private:
    string distribution_ = "uniform";
    int number_of_samples_ = 0;
    double y_ = 10;
    double geometric_param_ = 0.5;
    ExperimentEnvironment* experiment_environment_ = NULL;
    RecommenderDataIterator* recommender_data_iterator_ = NULL;
    RecommenderData local_recommender_data_;
    Random* random_ = NULL;
};

#endif
