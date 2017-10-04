#ifndef PERIODIC_OFFLINE_LEARNER_WRAPPER
#define PERIODIC_OFFLINE_LEARNER_WRAPPER

#include "../general_interfaces/Updater.h"
#include "../offline_learners/OfflineLearner.h"
#include "../utils/PeriodComputer.h"
#include "../models/Model.h"

struct PeriodicOfflineLearnerWrapperParameters{
  bool read_model = false;
  string base_in_file_name = "";
  bool write_model = false;
  string base_out_file_name = "";
  bool clear_model = false;
  bool learn = true;
};

class PeriodicOfflineLearnerWrapper : public Updater {
  public:
    PeriodicOfflineLearnerWrapper(PeriodicOfflineLearnerWrapperParameters* params){
      read_model_ = params->read_model;
      base_in_file_name_ = params->base_in_file_name;
      write_model_ = params->write_model;
      base_out_file_name_ = params->base_out_file_name;
      learn_ = params->learn;
    }
    void add_offline_learner(OfflineLearner* offline_learner){
      offline_learners_.push_back(offline_learner);
    }
    void set_model(Model* model){
      model_ = model;
    }
    void set_period_computer(PeriodComputer* period_computer){
      period_computer_ = period_computer;
    }
    void update(RecDat* rec_dat) override;
    bool self_test(){
      bool ok = Updater::self_test();
      if(period_computer_==NULL){ ok = false; cerr << "PeriodicOfflineLearnerWrapper::period_computer is not set." << endl; }
      if(model_==NULL and (read_model_ or write_model_ or clear_model_)){ ok = false; }
      return ok;
    }
  private:
    //components
    PeriodComputer* period_computer_ = NULL;
    vector<OfflineLearner*> offline_learners_;
    Model* model_ = NULL;
    //parameters
    bool read_model_ = false;
    string base_in_file_name_ = "";
    bool write_model_ = false;
    string base_out_file_name_ = "";
    bool clear_model_ = false;
    bool learn_ = true;
    //other
    void read_model(RecDat*);
    void clear_model(RecDat*);
    void write_model(RecDat*);
    void update_model(RecDat*);
    virtual RecommenderData* generate_recommender_data(RecDat* rec_dat){ return NULL; } //TODO should be a component to set?
};

#endif
