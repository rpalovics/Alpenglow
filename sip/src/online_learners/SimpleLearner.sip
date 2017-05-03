class SimpleLearner : public OnlineLearner {
  %TypeHeaderCode
  #include "online_learners/SimpleLearner.h"
  %End
  public:
    virtual void learn(RecDat* recDat);
    bool self_test();
};
