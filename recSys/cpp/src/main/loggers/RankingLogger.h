#ifndef RANKING_LOGGER
#define RANKING_LOGGER

#include <fstream>
#include "../online_recommender/OnlineRecommender.h"
#include "../ranking/Ranking.h"
#include "Logger.h"
struct RankingLoggerParameters{
  string out_file_name;
  double min_time;
};
class RankingLogger : public Logger {
  public:
    RankingLogger(RankingLoggerParameters* params){
      ofs.open(params->out_file_name.c_str());
      min_time_=params->min_time;
    }
    ~RankingLogger(){}
    void set_recommender(OnlineRecommender* recommender){ recommender_ = recommender; }
    void set_rank_computer(RankComputer* rank_computer){ rank_computer_ = rank_computer; }
    void run(RecDat* rec_dat){
      if(rec_dat->time >= min_time_ && rec_dat->eval==1){
        ofs << (int) rec_dat->time << " " << rank_computer_->get_rank(rec_dat) << " " << recommender_->prediction(rec_dat) << " " << rec_dat->user << " " << rec_dat->item << " " << rec_dat->score << " " << rec_dat->id << endl;
      }
    }
  private:
    ofstream ofs;
    double min_time_;
    OnlineRecommender* recommender_;
    RankComputer* rank_computer_;
};


#endif
