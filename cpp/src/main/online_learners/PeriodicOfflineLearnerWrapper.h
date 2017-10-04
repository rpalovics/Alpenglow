#ifndef PERIODIC_OFFLINE_LEARNER_WRAPPER
#define PERIODIC_OFFLINE_LEARNER_WRAPPER

#include "../general_interfaces/Updater.h"
#include "../offline_learners/OfflineLearner.h"
#include "../utils/PeriodComputer.h"

struct PeriodicOfflineLearnerWrapperParameters{
  bool read_model = false;
  string base_in_file_name = "";
  bool write_model = false;
  string base_out_file_name = "";
  bool clear_model = false;
};

class PeriodicOfflineLearnerWrapper : public Updater {
  public:
    PeriodicOfflineLearnerWrapper(PeriodicOfflineLearnerWrapperParameters* params){
      read_model_ = params->read_model;
      base_in_file_name_ = params->base_in_file_name;
      write_model_ = params->write_model;
      base_out_file_name_ = params->base_out_file_name;
    }
    void add_offline_learner(OfflineLearner* offline_learner){
      offline_learners_.push_back(offline_learner);
    }
    void set_period_computer(PeriodComputer* period_computer){
      period_computer_ = period_computer;
    }
    void update(RecDat* rec_dat) override;
    bool self_test(){
      bool ok = Updater::self_test();
      if(period_computer_==NULL){ ok = false; cerr << "PeriodicOfflineLearnerWrapper::period_computer is not set." << endl; }
      return ok;
    }
  private:
    //components
    PeriodComputer* period_computer_ = NULL;
    vector<OfflineLearner*> offline_learners_;
    //parameters
    bool read_model_ = false;
    string base_in_file_name_ = "";
    bool write_model_ = false;
    string base_out_file_name_ = "";
    bool clear_model_ = false;
    //other
    void read_model(RecDat*);
    void clear_model(RecDat*);
    void write_model(RecDat*);
    void update_model(RecDat*);
};

#endif
