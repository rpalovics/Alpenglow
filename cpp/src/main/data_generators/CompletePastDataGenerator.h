#ifndef COMPLETE_PAST_DATA_GENERATOR_H
#define COMPLETE_PAST_DATA_GENERATOR_H

//SIP_AUTOCONVERT

#include "DataGenerator.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"

class CompletePastDataGenerator : public DataGenerator, public NeedsExperimentEnvironment, public Initializable {
  public:
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
        recommender_data_iterator_=experiment_environment_->get_recommender_data_iterator();
      }
      return true;
    }
  private:
    const RecommenderDataIterator* recommender_data_iterator_ = NULL;
    RecommenderData local_recommender_data_;
};

#endif /* COMPLETE_PAST_DATA_GENERATOR_H */
