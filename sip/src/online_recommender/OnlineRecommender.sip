class OnlineRecommender{
  %TypeHeaderCode
  #include "online_recommender/OnlineRecommender.h"
  %End
  public:
    OnlineRecommender();
    ~OnlineRecommender();
    void set_model(Model* model);
    void set_learner(OnlineLearner* learner);
    void learn(RecDat * rec_dat);
    double prediction(RecDat * rec_dat);
    bool self_test();
};
