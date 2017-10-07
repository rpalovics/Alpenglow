#ifndef UNIFORM_NEGATIVE_SAMPLE_GENERATOR
#define UNIFORM_NEGATIVE_SAMPLE_GENERATOR

#include "NegativeSampleGenerator.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"
#include "../utils/Random.h"

struct UniformNegativeSampleGeneratorParameters{
    double negative_rate; 
    bool initialize_all;
    int max_item;
    bool filter_repeats;
    int seed=67439852;
    UniformNegativeSampleGeneratorParameters(){
      negative_rate=-1;
      initialize_all=false;
      max_item=-1;
      filter_repeats=false;
    }
};

class UniformNegativeSampleGenerator : public NegativeSampleGenerator, public Initializable, public NeedsExperimentEnvironment {
  public:
    UniformNegativeSampleGenerator(UniformNegativeSampleGeneratorParameters* parameters):
      negative_rate_(parameters->negative_rate),
      filter_repeats_(parameters->filter_repeats),
      initialize_all_(parameters->initialize_all),
      max_item_(parameters->max_item),
      rnd_(parameters->seed)
    {}
    void set_train_matrix(SpMatrix* train_matrix){ train_matrix_=train_matrix; }
    void set_items(vector<int>* items){ if(!initialize_all_) items_=items; }
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override {
      experiment_environment_=experiment_environment;
    }
      
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
      if(initialize_all_){ //TODO initialize all kozos parameter legyen
        items_=new vector<int>(max_item_+1);
        for(int i=0;i<items_->size();i++){items_->at(i)=i;}
      }
      if(items_==NULL){ items_=experiment_environment_->get_items(); }
      if(train_matrix_==NULL){ train_matrix_=experiment_environment_->get_train_matrix(); }
      return true;
    }
    vector<int>* items_ = NULL;
    SpMatrix* train_matrix_ = NULL; 
    ExperimentEnvironment* experiment_environment_;
    Random rnd_;
    const double negative_rate_;
    const bool filter_repeats_;
    const bool initialize_all_;
    const int max_item_;
};

#endif
