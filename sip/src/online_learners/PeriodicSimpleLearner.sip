struct PeriodicSimpleLearnerParameters : public PeriodicLearnerParameters{
  %TypeHeaderCode
  #include "online_learners/PeriodicSimpleLearner.h"
  %End
};

class PeriodicSimpleLearner : public PeriodicLearner {
  %TypeHeaderCode
  #include "online_learners/PeriodicSimpleLearner.h"
  %End
  public:
    PeriodicSimpleLearner(PeriodicSimpleLearnerParameters* params);
    virtual ~PeriodicSimpleLearner();
};