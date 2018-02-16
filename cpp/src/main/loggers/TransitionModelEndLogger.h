#ifndef TRANSITION_LOGGER_END_LOGGER
#define TRANSITION_LOGGER_END_LOGGER

#include <fstream>
#include <algorithm>
#include "Logger.h"
#include "../models/baseline/TransitionProbabilityModel.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../utils/PopContainers.h"
using namespace std;

struct TransitionModelEndLoggerParameters{
  string log_file_name; //output format: track len_of_list popularity track1 track2 ... track_max_length
  int max_length;
  TransitionModelEndLoggerParameters(){
    log_file_name="";
    max_length=-1;
  }
};
class TransitionModelEndLogger : public Logger, public NeedsExperimentEnvironment, public Initializable{
  public:
    TransitionModelEndLogger(TransitionModelEndLoggerParameters* params){
      model_=NULL;
      pop_container_=NULL;
      log_file_name_=params->log_file_name;
      max_length_=params->max_length;
    }
    void run(RecDat* rec_dat) override {
      ofstream ofs(log_file_name_.c_str());
      run_core(ofs);
    }
    void run_core(ostream& ofs){
      //vector<map<int,int>> transition_frequencies_;
      for(uint item=0;item<model_->transition_frequencies_.size();item++){
        int out_degree = model_->transition_frequencies_[item].size();
        int popularity = pop_container_->get(item);
        ofs << item << " " << out_degree << " " << popularity;
        vector<pair<int,int>> to_items;
        for(auto to_item = model_->transition_frequencies_[item].begin();to_item!=model_->transition_frequencies_[item].end();to_item++){
          to_items.push_back(make_pair(to_item->first,to_item->second));
        }
        sort(
            to_items.begin(),
            to_items.end(),
            [](pair<int,int> a, pair<int,int> b) -> bool
               { return (a.second) > (b.second); }
            );
        for(uint index=0;index<to_items.size() and index<max_length_;index++){
          ofs << " " << to_items[index].first << "," << to_items[index].second;
        }
        ofs << endl;
      }
    }
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override { experiment_environment_=experiment_environment; }
    void set_pop_container(PopContainer* pop_container){ pop_container_ = pop_container; }
    void set_model(TransitionProbabilityModel* model){ model_ = model; }
    bool self_test(){
      bool OK = Logger::self_test();
      if(model_==NULL){ OK=false; cerr << "TransitionModelEndLogger::model is not set." << endl; }
      if(pop_container_==NULL){ OK=false; cerr << "TransitionModelEndLogger::pop_container is not set." << endl; }
      return OK;
    }
  protected:
    bool autocalled_initialize() override {
      if (pop_container_==NULL) pop_container_=experiment_environment_->get_popularity_container();
      return true;
    }
  private:
    ExperimentEnvironment* experiment_environment_;
    TransitionProbabilityModel* model_;
    PopContainer* pop_container_;
    string log_file_name_;
    int max_length_;
};


#endif
