struct LearnerPeriodicDelayedWrapperParameters{
  %TypeHeaderCode
  #include "online_learners/LearnerPeriodicDelayedWrapper.h"
  %End
  double period;
  double delay;
};

class LearnerPeriodicDelayedWrapper : OnlineLearner{
  %TypeHeaderCode
  #include "online_learners/LearnerPeriodicDelayedWrapper.h"
  %End
  public:
    LearnerPeriodicDelayedWrapper(LearnerPeriodicDelayedWrapperParameters* params);
    virtual ~LearnerPeriodicDelayedWrapper();

    virtual void set_wrapped_learner(Updater* learner);

    virtual void learn(RecDat* rec_dat);
    virtual void add_simple_updater(ModelSimpleUpdater* model_updater);
    virtual void add_gradient_updater(ModelGradientUpdater* model_updater);
    virtual void add_multi_updater(ModelMultiUpdater* model_updater);
    virtual void set_model(Model* model);

    bool self_test();
};
