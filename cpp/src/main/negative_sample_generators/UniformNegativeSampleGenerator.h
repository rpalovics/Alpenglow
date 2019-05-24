#ifndef UNIFORM_NEGATIVE_SAMPLE_GENERATOR_H
#define UNIFORM_NEGATIVE_SAMPLE_GENERATOR_H

#include "NegativeSampleGenerator.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"
#include "../utils/Random.h"

struct UniformNegativeSampleGeneratorParameters {
    double negative_rate = -1; 
    int initialize_all = -1;
    int max_item = -1;
    bool filter_repeats = false;
    int seed=67439852;
};

class UniformNegativeSampleGenerator : public NegativeSampleGenerator, public Initializable, public NeedsExperimentEnvironment {
  public:
    UniformNegativeSampleGenerator(UniformNegativeSampleGeneratorParameters* parameters):
      negative_rate_(parameters->negative_rate),
      filter_repeats_(parameters->filter_repeats),
      initialize_all_(parameters->initialize_all),
      max_item_(parameters->max_item),
      random_(parameters->seed)
    {}
    ~UniformNegativeSampleGenerator(){
      if(local_items_!=NULL) delete local_items_;
    }
    void set_train_matrix(SpMatrix* train_matrix){ train_matrix_=train_matrix; }
    void set_items(vector<int>* items){ if(!initialize_all_) items_=items; }
      
    vector<int>* generate(RecDat* rec_dat);
    bool self_test(){
      bool ok = NegativeSampleGenerator::self_test();
      if(negative_rate_ < 0){
        ok=false;
        cerr << "UniformNegativeSampleGeneratorParameters::negative_rate is negative." << endl;
      }
      if(initialize_all_ && max_item_<0){
        ok = false;
        cerr << "UniformNegativeSampleGeneratorParameters::max_item is negative but initialize_all is set." << endl;
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
  protected:
    bool autocalled_initialize() override {
      if(experiment_environment_==NULL && (
          initialize_all_==-1
          || initialize_all_ && max_item_==-1
          || !initialize_all_ && items_==NULL
          || train_matrix_==NULL
          )){
        return false;
      }
      if(initialize_all_==-1){
        initialize_all_=experiment_environment_->get_initialize_all();
      }
      if(initialize_all_){ //TODO initialize all kozos parameter legyen
        if (max_item_==-1) max_item_=experiment_environment_->get_max_item_id();
        local_items_ = new vector<int>(max_item_+1);
        for(uint i=0;i<local_items_->size();i++){local_items_->at(i)=i;}
        items_=local_items_;
      }
      if(items_==NULL){ items_=experiment_environment_->get_items(); }
      if(train_matrix_==NULL){ train_matrix_=experiment_environment_->get_train_matrix(); }
      return true;
    }
    const vector<int>* items_ = NULL;
    vector<int> indices_;
    vector<int>* local_items_ = NULL;
    const SpMatrix* train_matrix_ = NULL; 
    Random random_;
    const double negative_rate_;
    const bool filter_repeats_;
    int initialize_all_;
    int max_item_;
};

#endif /* UNIFORM_NEGATIVE_SAMPLE_GENERATOR_H */
