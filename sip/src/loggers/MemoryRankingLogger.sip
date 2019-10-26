
struct RankingLog{
  %TypeHeaderCode
  #include "loggers/MemoryRankingLogger.h"
  %End
  double time;
  int id;
  int rank;
  int user;
  int item;
  double score;
  double prediction;
};

struct RankingLogs{
  %TypeHeaderCode
  #include "loggers/MemoryRankingLogger.h"
  %End
  int top_k;
  std::vector<RankingLog> logs;
};

struct MemoryRankingLoggerParameters{
  %TypeHeaderCode
  #include "loggers/MemoryRankingLogger.h"
  %End
  std::string out_file;
  bool memory_log;
  double evaluation_start_time;
  int top_k;
  int random_seed;
};

class MemoryRankingLogger
  : public Logger
  , public NeedsExperimentEnvironment
  , public Initializable /NoDefaultCtors/
  {
  %TypeHeaderCode
  #include "loggers/MemoryRankingLogger.h"
  %End
  public:
    MemoryRankingLogger(MemoryRankingLoggerParameters* params);
    void set_model(Model* model);
    void set_ranking_logs(RankingLogs* logs);
    const RankingLogs* get_ranking_logs() const;

    void set_train_matrix(SpMatrix* train_matrix);
    void set_items(std::vector<int> *items);
    void set_top_pop_container(TopPopContainer* popularity_sorted_container);

    void run(RecDat* rec_dat);
    bool self_test();
  protected:
    bool autocalled_initialize();
};

