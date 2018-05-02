#ifndef UNIFORM_NEGATIVE_SAMPLE_GENERATOR_H
#define UNIFORM_NEGATIVE_SAMPLE_GENERATOR_H

#include "NegativeSampleGenerator.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"
#include "../utils/Random.h"

//SIP_AUTOCONVERT
struct UniformNegativeSampleGeneratorParameters {
    double negative_rate = -1; 
    bool filter_repeats = false;
    int seed=67439852;
};

class UniformNegativeSampleGenerator : public NegativeSampleGenerator, public Initializable, public NeedsExperimentEnvironment {
  public:
    UniformNegativeSampleGenerator(UniformNegativeSampleGeneratorParameters* parameters):
      negative_rate_(parameters->negative_rate),
      filter_repeats_(parameters->filter_repeats),
      random_(parameters->seed)
    {}
    void set_train_matrix(SpMatrix* train_matrix){ train_matrix_=train_matrix; }
    void set_items(vector<int>* items){ items_=items; }
    /* SIP_CODE
    void set_items(VectorInt);
    %MethodCode
        sipCpp->set_items(&(a0->vec));
    %End
    */
      
    vector<int>* generate(RecDat* rec_dat);
    bool self_test(){
      bool ok = NegativeSampleGenerator::self_test() && random_.self_test();
      if(negative_rate_ < 0){
        ok=false;
        cerr << "UniformNegativeSampleGeneratorParameters::negative_rate is negative." << endl;
      }
      if(items_==NULL){
        ok = false;
        cerr << "UniformNegativeSampleGeneratorParameters::items is not set." << endl;
      }
      if(train_matrix_ == NULL){
        ok = false;
        cerr << "UniformNegativeSampleGeneratorParameters::train_matrix is not set." << endl;
      }
      return ok;
    }
    virtual void message(UpdaterMessage message) override {
      switch(message){
        case UpdaterMessage::start_of_offline_update:
          cout << "start_of_offline_update" << endl;
          break;
        case UpdaterMessage::end_of_offline_update:
          cout << "end_of_offline_update" << endl;
          break;
        case UpdaterMessage::start_of_offline_iterations:
          cout << "start_of_offline_iterations" << endl;
          break;
        case UpdaterMessage::end_of_offline_iterations:
          cout << "end_of_offline_iterations" << endl;
          break;
        case UpdaterMessage::start_of_implicit_update_cycle:
          cout << "start_of_implicit_update_cycle" << endl;
          break;
        case UpdaterMessage::end_of_implicit_update_cycle:
          cout << "end_of_implicit_update_cycle" << endl;
          break;
      }
      if(message == UpdaterMessage::start_of_offline_iterations){
        cout << "items size: " << items_->size() << endl;
        cout << "trainmat size: " << train_matrix_->size() << endl;
      }
    }
  protected:
    bool autocalled_initialize() override {
      if(experiment_environment_==NULL && (
          items_==NULL
          || train_matrix_==NULL
          )){
        return false;
      }
      if(items_==NULL){ items_=experiment_environment_->get_items(); }
      if(train_matrix_==NULL){ train_matrix_=experiment_environment_->get_train_matrix(); }
      return true;
    }
    const vector<int>* items_ = NULL;
    vector<int> indices_;
    const SpMatrix* train_matrix_ = NULL; 
    Random random_;
    const double negative_rate_;
    const bool filter_repeats_;
};

#endif /* UNIFORM_NEGATIVE_SAMPLE_GENERATOR_H */
