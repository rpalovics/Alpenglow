struct LearnerPeriodicDelayedWrapperParameters{
  %TypeHeaderCode
  #include "online_learners/LearnerPeriodicDelayedWrapper.h"
  %End
  double period;
  double delay;
};

class LearnerPeriodicDelayedWrapper : Updater{
  %TypeHeaderCode
  #include "online_learners/LearnerPeriodicDelayedWrapper.h"
  %End
  public:
    LearnerPeriodicDelayedWrapper(LearnerPeriodicDelayedWrapperParameters* params);
    virtual ~LearnerPeriodicDelayedWrapper();

    void set_wrapped_learner(Updater* learner);

    virtual void update(RecDat* rec_dat);

    bool self_test();
};