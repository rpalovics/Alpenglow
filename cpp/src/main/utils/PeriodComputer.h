#ifndef PERIOD_COMPUTER
#define PREIOD_COMPUTER

#include <string>
#include "../recommender_data/RecommenderData.h"
#include "../general_interfaces/Updater.h"
#include "../general_interfaces/INeedExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"

using namespace std;

struct PeriodComputerParameters{
  string mode = "time"; //"samplenum": we treat sample counter as time
  int period_length = 86400;
  int start_time = -1; //if time<start_time, end_of_period will be false
};

class PeriodComputer : public Updater, public INeedExperimentEnvironment, public Initializable {
  public:
    PeriodComputer(PeriodComputerParameters* params){ set_parameters(params); }
    PeriodComputer(){}
    void set_parameters(PeriodComputerParameters* params){
      mode_ = params->mode;
      period_length_ = params->period_length;
      start_time_ = params->start_time;
    }
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override { experiment_environment_=experiment_environment; }
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator){ recommender_data_iterator_ = recommender_data_iterator; }
    bool end_of_period() const; //period num change at the last update
    void update(RecDat* rec_dat);
    int get_period_num() const;
    bool self_test(){
      bool ok = Updater::self_test();
      if (recommender_data_iterator_==NULL) ok=false;
      if (mode_ != "time" and mode_ != "samplenum") ok=false;
      return ok;
    }
  protected:
    bool autocalled_initialize() override {
      if(recommender_data_iterator_==NULL) recommender_data_iterator_ = experiment_environment_->get_recommender_data_iterator();
      if(mode_=="time"){
        if(!recommender_data_iterator_->is_initialized()) return false;
        timestamp_ = recommender_data_iterator_->get_following_timestamp();
        next_period_num_ = timestamp_/period_length_;
      }
      return true;
    }
  private:
    ExperimentEnvironment* experiment_environment_ = NULL;
    RecommenderDataIterator* recommender_data_iterator_ = NULL;
    //state
    int next_period_num_ = 0;
    int period_num_ = 0; 
    double timestamp_ = 0;
    bool reached_end_ = false;
    //parameters TODO const...
    string mode_ = "time";
    int period_length_ = 86400;
    int start_time_ = -1;
};

#endif
