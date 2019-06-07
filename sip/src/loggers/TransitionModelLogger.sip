
struct TransitionModelLoggerParameters {
  %TypeHeaderCode
  #include "loggers/TransitionModelLogger.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.TransitionModelLogger`. See documentation there.
  %End
  std::string toplist_length_logfile_basename; 
  std::string timeline_logfile_name;
  int period_length;
  int top_k;
};
class TransitionModelLogger
  : public Logger
  , public NeedsExperimentEnvironment
  , public Initializable
   /NoDefaultCtors/{
  %TypeHeaderCode
  #include "loggers/TransitionModelLogger.h"
  %End
  public:
    TransitionModelLogger(TransitionModelLoggerParameters* params);
    void run(RecDat* rec_dat);
    void set_pop_container(PopContainer* pop_container);
    void set_model(TransitionProbabilityModel* model);
    void set_train_matrix(SpMatrix* train_matrix);
    bool self_test();
  protected:
    bool autocalled_initialize();
};

