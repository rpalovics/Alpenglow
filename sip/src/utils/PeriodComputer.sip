
struct PeriodComputerParameters{
  %TypeHeaderCode
  #include "utils/PeriodComputer.h"
  %End
  std::string period_mode; 
  int period_length;
  int start_time; 
};

class PeriodComputer : public Updater, public NeedsExperimentEnvironment, public Initializable {
  %TypeHeaderCode
  #include "utils/PeriodComputer.h"
  %End
  public:
    PeriodComputer(PeriodComputerParameters* params);
    PeriodComputer();
    void set_parameters(PeriodComputerParameters* params);
    void set_experiment_environment(ExperimentEnvironment* experiment_environment);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    bool end_of_period() const; 
    void update(RecDat* rec_dat);
    int get_period_num() const;
    bool self_test();
  protected:
    bool autocalled_initialize();
};

