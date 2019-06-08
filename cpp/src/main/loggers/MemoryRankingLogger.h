#ifndef MEMORY_RANKING_LOGGER_H
#define MEMORY_RANKING_LOGGER_H

#include "../models/Model.h"
#include "../utils/RankComputer.h"
#include "Logger.h"
#include <vector>
#include <fstream>

struct RankingLog{
  double time;
  int id;
  int rank;
  int user;
  int item;
  double score;
  double prediction;
};

struct RankingLogs{
  int top_k;
  vector<RankingLog> logs;
};

struct MemoryRankingLoggerParameters{
  string out_file="";
  bool memory_log = true;
  double evaluation_start_time = -1;
  int top_k = -1;
  int random_seed = -1;
};

class MemoryRankingLogger
  : public Logger
  , public NeedsExperimentEnvironment
  , public Initializable
  {
  public:
    MemoryRankingLogger(MemoryRankingLoggerParameters* params){
      evaluation_start_time_=params->evaluation_start_time;
      out_file_name_=params->out_file;
      if(out_file_name_ != ""){
        ofs.open(out_file_name_.c_str());
      }
      memory_log_ = params->memory_log;
      RankComputerParameters rank_computer_parameters;
      rank_computer_parameters.top_k = params->top_k;
      rank_computer_parameters.random_seed = params->random_seed;
      rank_computer_.set_parameters(&rank_computer_parameters);
    }
    void set_model(Model* model){
      model_ = model;
      rank_computer_.set_model(model);
    }
    void set_ranking_logs(RankingLogs* logs){ logs_=logs; } //TODO create log locally
    const RankingLogs* get_ranking_logs() const { return logs_; }

    void set_model_filter(ModelFilter* model_filter){ rank_computer_.set_model_filter(model_filter); }
    void set_train_matrix(SpMatrix* train_matrix){ rank_computer_.set_train_matrix(train_matrix); }
    void set_items(vector<int> *items){ rank_computer_.set_items(items); }
    void set_top_pop_container(TopPopContainer* popularity_sorted_container){
      rank_computer_.set_top_pop_container(popularity_sorted_container);
    }

    void run(RecDat* rec_dat) override;
    bool self_test(){
      bool ok = Logger::self_test() && rank_computer_.self_test();
      if (model_==NULL) ok=false;
      if (memory_log_ && logs_==NULL) ok=false; //TODO create log locally
      if (out_file_name_!="" && !ofs.is_open()) ok=false;
      return ok;
    }
  protected:
    bool autocalled_initialize() override {
      rank_computer_.set_experiment_environment(experiment_environment_);
      return rank_computer_.initialize();
    }
  private:
    ofstream ofs;
    RankingLogs* logs_ = NULL; //TODO create log locally
    double evaluation_start_time_;
    Model* model_ = NULL;
    RankComputer rank_computer_;
    bool memory_log_;
    string out_file_name_;
};

#endif /* MEMORY_RANKING_LOGGER_H */
