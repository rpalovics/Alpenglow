struct PeriodicTimeframeImplicitGradientLearnerParameters : public PeriodicImplicitGradientLearnerParameters {
  %TypeHeaderCode
  #include "online_learners/PeriodicTimeframeImplicitGradientLearner.h"
  %End
  int timeframe_length;
};

class PeriodicTimeframeImplicitGradientLearner : public PeriodicImplicitGradientLearner {
  %TypeHeaderCode
  #include "online_learners/PeriodicTimeframeImplicitGradientLearner.h"
  %End
  public:
    PeriodicTimeframeImplicitGradientLearner(PeriodicTimeframeImplicitGradientLearnerParameters* params); 
    void set_local_items(std::vector<int>* items);
    void set_local_items(VectorInt);
    %MethodCode
        sipCpp->set_local_items(&(a0->vec));
    %End
    void set_local_train_matrix(SpMatrix* train_matrix);
    bool self_test();
};
