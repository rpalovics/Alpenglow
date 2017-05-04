class CombinedDoubleLayerLearner : public OnlineLearner{
  %TypeHeaderCode
  #include "online_learners/CombinedDoubleLayerLearner.h"
  %End
  public:
    CombinedDoubleLayerLearner(); 
    void add_learner(OnlineLearner* learner);
    void set_meta_learner(OnlineLearner* learner);
    void learn(RecDat* recDat);
    bool self_test();
    void addLearner(OnlineLearner* learner);
    void setMetaLearner(OnlineLearner* learner);
};