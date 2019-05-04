#ifndef MEMORY_RANKING_LOGGER_H
#define MEMORY_RANKING_LOGGER_H

#include "models/Model.h"
#include "utils/Ranking.h"
#include "loggers/Logger.h"
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
  double min_time = 0; //TODO ExpEnv
  string out_file="";
  bool memory_log = true;
  int top_k = 0; //TODO ExpEnv
  int random_seed = 1; //TODO ExpEnv
};

class MemoryRankingLogger
  : public Logger
  , public NeedsExperimentEnvironment
  , public Initializable
  {
  public:
    MemoryRankingLogger(MemoryRankingLoggerParameters* params){
      min_time_=params->min_time;
      if(params->out_file != ""){
        ofs.open(params->out_file.c_str());
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
    void set_ranking_logs(RankingLogs* logs){ logs_=logs; } //TODO
    const RankingLogs* get_ranking_logs() const { return logs_; } //TODO

    void set_model_filter(ModelFilter* model_filter){ rank_computer_.set_model_filter(model_filter); }
    void set_train_matrix(SpMatrix* train_matrix){ rank_computer_.set_train_matrix(train_matrix); }
    void set_items(vector<int> *items){ rank_computer_.set_items(items); }
    void set_top_pop_container(TopPopContainer* popularity_sorted_container){
      rank_computer_.set_top_pop_container(popularity_sorted_container);
    }
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override {
      rank_computer_.set_experiment_environment(experiment_environment);
    }

    void run(RecDat* rec_dat){
      if(rec_dat->time >= min_time_ && rec_dat->eval==1){
        RankingLog log;
        log.time = rec_dat->time;
        log.rank = rank_computer_.get_rank(rec_dat);
        log.prediction = model_->prediction(rec_dat);
        log.item = rec_dat->item;
        log.user = rec_dat->user;
        log.score = rec_dat->score;
        log.id = rec_dat->id;
        if(memory_log_){
          logs_->logs.push_back(log);
        }

        if(ofs.is_open()){
          ofs 
            << (int) rec_dat->time << " "
            << log.rank << " "
            << log.prediction << " "
            << rec_dat->user << " "
            << rec_dat->item << " "
            << rec_dat->score << " "
            << rec_dat->id << endl;
        }
      }
    }
    bool self_test(){
      return rank_computer_.self_test();
    }
  protected:
    bool autocalled_initialize() override {
      return rank_computer_.initialize();
    }
  private:
    ofstream ofs;
    RankingLogs* logs_ = NULL; //TODO local variable
    double min_time_;
    Model* model_ = NULL;
    RankComputer rank_computer_;
    bool memory_log_;
};

#endif
