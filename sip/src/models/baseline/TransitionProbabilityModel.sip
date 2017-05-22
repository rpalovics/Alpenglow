class TransitionProbabilityModel: public Model, public ModelFilter{
  %TypeHeaderCode
  #include "models/baseline/TransitionProbabilityModel.h"
  %End
  public:
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
    // std::vector<pair<int,double>>* get_personalized_items(int user);
    bool self_test();
};
