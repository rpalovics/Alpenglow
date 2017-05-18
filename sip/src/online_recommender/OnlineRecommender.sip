class OnlineRecommender{
  %TypeHeaderCode
  #include "online_recommender/OnlineRecommender.h"
  %End
  public:
    OnlineRecommender();
    ~OnlineRecommender();
    void set_model(Model* model);
    void set_learner(OnlineLearner* learner);
    void learn(RecDat * recDat);
    double prediction(RecDat * recDat);
    bool self_test();
    void writeParameters();
};
