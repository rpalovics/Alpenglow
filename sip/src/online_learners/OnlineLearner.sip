class OnlineLearner {
  %TypeHeaderCode
  #include "online_learners/OnlineLearner.h"
  %End
  public:
    OnlineLearner();
    virtual ~OnlineLearner();
    virtual void learn(RecDat* rec_dat)=0;
    void add_simple_updater(ModelSimpleUpdater* model_updater);
    void add_gradient_updater(ModelGradientUpdater* model_updater);
    void add_multi_updater(ModelMultiUpdater* model_updater);
    void set_model(Model* model);
    bool self_test();
};
