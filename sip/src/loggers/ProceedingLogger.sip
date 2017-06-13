class ProceedingLogger : public Logger, public Initializable, public INeedExperimentEnvironment {
  %TypeHeaderCode
  #include "loggers/ProceedingLogger.h"
  %End
  public:
    ProceedingLogger();
    virtual ~ProceedingLogger();
    virtual void run(RecDat* rec_dat);
    virtual bool init();
    virtual void set_experiment_environment(ExperimentEnvironment* experiment_environment);
    void set_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    bool self_test();
};
