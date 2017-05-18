class SimpleLearner : public OnlineLearner {
  %TypeHeaderCode
  #include "online_learners/SimpleLearner.h"
  %End
  public:
    virtual void learn(RecDat* rec_dat);
    bool self_test();
};
