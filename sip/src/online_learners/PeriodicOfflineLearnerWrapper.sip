struct PeriodicOfflineLearnerWrapperParameters{
  %TypeHeaderCode
  #include "online_learners/PeriodicOfflineLearnerWrapper.h"
  %End
  bool read_model;
  std::string base_in_file_name;
  bool write_model;
  std::string base_out_file_name;
  bool clear_model;
  bool learn;
};

class PeriodicOfflineLearnerWrapper : public Updater {
  %TypeHeaderCode
  #include "online_learners/PeriodicOfflineLearnerWrapper.h"
  %End
  public:
    PeriodicOfflineLearnerWrapper(PeriodicOfflineLearnerWrapperParameters* params);
    void add_offline_learner(OfflineLearner* offline_learner);
    void set_model(Model* model);
    void set_period_computer(PeriodComputer* period_computer);
    void set_data_generator(DataGenerator* data_generator);
    void update(RecDat* rec_dat);
    bool self_test();
};
