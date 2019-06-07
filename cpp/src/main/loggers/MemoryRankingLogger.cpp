#include "MemoryRankingLogger.h"

void MemoryRankingLogger::run(RecDat* rec_dat){
  if(rec_dat->time >= evaluation_start_time_ && rec_dat->eval==1){
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
