struct RankingLoggerParameters{
  %TypeHeaderCode
  #include "loggers/RankingLogger.h"
  %End
  std::string out_file_name;
  double min_time;
};
class RankingLogger : public Logger /NoDefaultCtors/{
  %TypeHeaderCode
  #include "loggers/RankingLogger.h"
  %End
  public:
    RankingLogger(RankingLoggerParameters* params);
    ~RankingLogger();
    void set_recommender(OnlineRecommender* recommender /Transfer/);
    void set_rank_computer(RankComputer* rank_computer /Transfer/);
    void run(RecDat* rec_dat);
};
