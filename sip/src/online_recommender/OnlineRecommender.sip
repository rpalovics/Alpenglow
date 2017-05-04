class OnlineRecommender{
  %TypeHeaderCode
  #include "online_recommender/OnlineRecommender.h"
  %End
  public:
    OnlineRecommender();
    ~OnlineRecommender();
    void set_model(Model* model /Transfer/);
    void set_learner(OnlineLearner* learner /Transfer/);
    void learn(RecDat * recDat);
    double prediction(RecDat * recDat);
    bool selfTest();
    void writeParameters();
};
