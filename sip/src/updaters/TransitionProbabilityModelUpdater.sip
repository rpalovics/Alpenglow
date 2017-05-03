struct TransitionProbabilityModelUpdaterParameters{
  %TypeHeaderCode
  #include "updaters/TransitionProbabilityModelUpdater.h"
  %End
  bool filter_freq_updates;
  std::string mode;
  bool label_transition_mode;
  std::string label_file_name;
  TransitionProbabilityModelUpdaterParameters();
};

class TransitionProbabilityModelUpdater: public ModelSimpleUpdater{
  %TypeHeaderCode
  #include "updaters/TransitionProbabilityModelUpdater.h"
  %End
  public:
    TransitionProbabilityModelUpdater(TransitionProbabilityModelUpdaterParameters* params);
    void update(RecDat* rec_dat);
    void set_model(TransitionProbabilityModel* model);
    bool self_test();
};
