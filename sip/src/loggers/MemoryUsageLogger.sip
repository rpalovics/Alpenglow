
class MemoryUsageLogger : public Logger, public Initializable, public NeedsExperimentEnvironment{
  %TypeHeaderCode
  #include "loggers/MemoryUsageLogger.h"
  %End
  public:
    MemoryUsageLogger();
    virtual ~MemoryUsageLogger();
    void run(RecDat* rec_dat);
    void set_experiment_environment(ExperimentEnvironment* experiment_environment);
    void set_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    bool self_test();
  protected:
    bool autocalled_initialize();
};

