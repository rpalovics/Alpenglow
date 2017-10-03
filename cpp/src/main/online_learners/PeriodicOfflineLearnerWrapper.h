#ifndef PERIODIC_OFFLINE_LEARNER_WRAPPER
#define PERIODIC_OFFLINE_LEARNER_WRAPPER

#include "../general_interfaces/Updater.h"
#include "../offline_learners/OfflineLearner.h"
#include "../utils/PeriodComputer.h"

struct PeriodicOfflineLearnerWrapperParameters{
  int period_length; //time or samplenum
  string mode; //time or samplenum
  int start_time;
  bool read_model;
  string base_in_file_name;
  bool write_model;
  string base_out_file_name;
  bool clear_model;
};

class PeriodicOfflineLearnerWrapper : public Updater , public INeedExperimentEnvironment, public Initializable{
  public:
    PeriodicOfflineLearnerWrapper(PeriodicOfflineLearnerWrapperParameters* params){
    }
    void set_offline_learner(OfflineLearner* offline_learner){
      offline_learner_ = offline_learner;
    }
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override { experiment_environment_=experiment_environment; } //TODO period_computer_
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){ recommender_data_iterator_ = recommender_data_iterator; } //TODO period_computer_
    void update(RecDat* rec_dat) override;
    bool self_test(){
      bool ok = Updater::self_test();
      if(offline_learner_==NULL){ ok = false; cerr << "PeriodicOfflineLearnerWrapper::offline_learner is not set." << endl; }
      return ok;
    }
  private:
    ExperimentEnvironment* experiment_environment_ = NULL;
    RecommenderDataIterator* recommender_data_iterator_ = NULL;
    PeriodComputer period_computer_;
    OfflineLearner* offline_learner_ = NULL;
    void read_model(RecDat*);
    void clear_model(RecDat*);
    void write_model(RecDat*);
    void update_model(RecDat*);
};

#endif
