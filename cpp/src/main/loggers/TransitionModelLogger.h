#ifndef TRANSITION_MODEL_LOGGER
#define TRANSITION_MODEL_LOGGER

#include <fstream>
#include <algorithm>
#include <numeric>
#include "Logger.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../models/baseline/TransitionProbabilityModel.h"
#include "../utils/PopContainers.h"
using namespace std;

struct TransitionModelLoggerParameters{
  string toplist_length_logfile_basename; //output format: track toplist_len
  string timeline_logfile_name;
  int period_length;
  TransitionModelLoggerParameters(){
    toplist_length_logfile_basename="";
    timeline_logfile_name="";
    period_length=-1;
  }
};
class TransitionModelLogger : public Logger, public NeedsExperimentEnvironment, public Initializable{
  public:
    TransitionModelLogger(TransitionModelLoggerParameters* params):
      period_length_(params->period_length)
    {
      model_=NULL;
      pop_container_=NULL;
      train_matrix_=NULL;
      toplist_length_logfile_basename_=params->toplist_length_logfile_basename;
      timeline_logfile_name_=params->timeline_logfile_name;
      last_period_num_=0;
    }
    void run(RecDat* rec_dat) override {
      int period_num = (int)rec_dat->time/period_length_;
      if(period_num!=last_period_num_){
        write_toplist_lengths(rec_dat);
        last_period_num_=period_num;
      }
      write_eval_statistics(rec_dat);
    }
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override { experiment_environment_=experiment_environment; }
    void set_pop_container(PopContainer* pop_container){ pop_container_ = pop_container; }
    void set_model(TransitionProbabilityModel* model){ model_ = model; }
    void set_train_matrix(SpMatrix* train_matrix){ train_matrix_ = train_matrix; }
    bool self_test(){
      bool OK = Logger::self_test();
      if(model_==NULL){ OK=false; cerr << "TransitionModelLogger::model is not set." << endl; }
      if(pop_container_==NULL){ OK=false; cerr << "TransitionModelLogger::pop_container is not set." << endl; }
      if(train_matrix_==NULL){ OK=false; cerr << "TransitionModelLogger::train_matrix is not set." << endl; }
      return OK;
    }
  protected:
    bool autocalled_initialize() override {
      if (pop_container_==NULL) pop_container_=experiment_environment_->get_popularity_container();
      if (train_matrix_==NULL) train_matrix_=experiment_environment_->get_train_matrix();
      timeline_file_.open(timeline_logfile_name_);
      return true;
    }
  private:
    //params
    string toplist_length_logfile_basename_;
    string timeline_logfile_name_;
    const int period_length_;
    //state
    int last_period_num_;
    //other
    ofstream timeline_file_;
    ExperimentEnvironment* experiment_environment_;
    TransitionProbabilityModel* model_;
    PopContainer* pop_container_;
    SpMatrix* train_matrix_;
    void write_eval_statistics(RecDat* rec_dat){ 
      if(rec_dat->eval==0) return;
      int user = rec_dat->user;
      int item = rec_dat->item;
      auto actual_frequency_map = model_->get_frequency_map(user);
      vector<pair<int,int>> filtered_sorted_frequency_list = compute_toplist(actual_frequency_map, rec_dat);
      int toplist_length = filtered_sorted_frequency_list.size();
      int from_pop = accumulate(filtered_sorted_frequency_list.begin(),filtered_sorted_frequency_list.end(),0.0,[](double a, pair<int,int> b){ return a+b.second; });
      int to_pop = pop_container_->get(item);
      double actual_prediction = model_->prediction(rec_dat);
      int first_score = (filtered_sorted_frequency_list.size()==0?0:filtered_sorted_frequency_list[0].second); //TODO ez most freq, nem score
      int second_score = (filtered_sorted_frequency_list.size()<2?0:filtered_sorted_frequency_list[1].second); //TODO ez most freq, nem score
      int last_score = (filtered_sorted_frequency_list.size()<101?0:filtered_sorted_frequency_list[99].second); //TODO top_k szerint kene, 100 bele van egetve
      timeline_file_ << (int)rec_dat->time << " " << rec_dat->eval << " " << rec_dat->id << " " << item << " " << toplist_length << " " << from_pop << " " << to_pop << " " << actual_prediction << " " << first_score << " " << second_score << " " << last_score << endl;
    }
    vector<pair<int,int>> compute_toplist(map<int,int>* actual_frequency_map, RecDat* rec_dat){
      vector<pair<int,int>> toplist;
      if (actual_frequency_map==NULL) return toplist; //empty list
      int user = rec_dat->user;
      for (auto& frequency : *actual_frequency_map){
        if(train_matrix_->get(user,frequency.first)<1){
          toplist.push_back(frequency);
        }
      }
      sort(
          toplist.begin(),
          toplist.end(),
          [](pair<int,int> a, pair<int,int> b) -> bool
             { return (a.second) > (b.second); }
          );
      return toplist;
    }
    void write_toplist_lengths(RecDat* rec_dat){
      string filename=toplist_length_logfile_basename_+"_"+to_string(last_period_num_);
      ofstream ofs(filename.c_str());
      for(uint item=0;item<model_->transition_frequencies_.size();item++){
        int out_degree = model_->transition_frequencies_[item].size();
        //int popularity = pop_container_->get(item);
        ofs << item << " " << out_degree;// << " " << popularity;
        ofs << endl;
      }
    }
};


#endif
