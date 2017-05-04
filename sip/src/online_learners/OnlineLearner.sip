class OnlineLearner {
  %TypeHeaderCode
  #include "online_learners/OnlineLearner.h"
  %End
  public:
    OnlineLearner();
    virtual ~OnlineLearner();
    virtual void learn(RecDat* rec_dat)=0;
    void add_simple_updater(ModelSimpleUpdater* model_updater /Transfer/);
    void add_gradient_updater(ModelGradientUpdater* model_updater /Transfer/);
    void add_multi_updater(ModelMultiUpdater* model_updater /Transfer/);
    void set_model(Model* model /Transfer/);
    bool self_test();

    //DEPRECATED
    void addSimpleUpdater(ModelSimpleUpdater* model_updater /Transfer/);
    void addGradientUpdater(ModelGradientUpdater* model_updater /Transfer/);
    void setModel(Model* model /Transfer/);
    bool selfTest();
};
