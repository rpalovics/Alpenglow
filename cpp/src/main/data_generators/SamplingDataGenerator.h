#ifndef SAMPLING_DATA_GENERATOR_H
#define SAMPLING_DATA_GENERATOR_H

//SIP_AUTOCONVERT

#include "DataGenerator.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"

struct SamplingDataGeneratorParameters {
  string distribution = "uniform";
  int number_of_samples = 1000;
  double y = 10; //parameter of arctg random
  double geometric_param = 0.5; //parameter of geometric random
  int seed = 745578;
};
class SamplingDataGenerator : public DataGenerator, public Initializable, public NeedsExperimentEnvironment {
  public:
    SamplingDataGenerator(SamplingDataGeneratorParameters* params){
      distribution_ = params->distribution;
      number_of_samples_ = params->number_of_samples;
      y_ = params->y;
      geometric_param_ = params->geometric_param;
      random_.set(params->seed);
    }
    RecommenderData* generate_recommender_data(RecDat*) override;
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){
     recommender_data_iterator_ = recommender_data_iterator;
    }
    bool self_test(){
      bool ok=DataGenerator::self_test();
      if(!random_.self_test()) ok=false;
      if(recommender_data_iterator_==NULL) ok=false;
      if(number_of_samples_<0) ok=false;
      if(geometric_param_>1) ok=false; //it's a probability value...
      if(distribution_!="uniform"
         and distribution_!="linear"
         and distribution_!="arctg"
         and distribution_!="geometric") ok=false;
      return ok;
    }
  protected:
    bool autocalled_initialize() override {
      if(recommender_data_iterator_==NULL){
        if (experiment_environment_==NULL) return false;
        recommender_data_iterator_=experiment_environment_->get_recommender_data_iterator();
      }
      return true;
    }
  private:
    string distribution_;
    int number_of_samples_;
    double y_;
    double geometric_param_;
    const RecommenderDataIterator* recommender_data_iterator_ = NULL;
    RecommenderData local_recommender_data_;
    Random random_;
};

#endif /* SAMPLING_DATA_GENERATOR_H */
