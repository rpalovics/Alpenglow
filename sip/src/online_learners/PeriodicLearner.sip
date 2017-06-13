struct PeriodicLearnerParameters{
  %TypeHeaderCode
  #include "online_learners/PeriodicLearner.h"
  %End
  int period_length;
  int start_time;
  bool read_model;
  bool write_model;
  std::string base_in_file_name;
  std::string base_out_file_name;
  bool clear_model;
  std::string mode;
  int samplenum_in_periods;
};

class PeriodicLearner : public OnlineLearner, public Initializable {
  %TypeHeaderCode
  #include "online_learners/PeriodicLearner.h"
  %End
  public:
    PeriodicLearner(PeriodicLearnerParameters* params);
    virtual ~PeriodicLearner();
    void learn(RecDat* rec_dat);
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data);
    bool init();
    bool self_test();
};