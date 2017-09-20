struct OfflineIteratingLearnerParameters {
  %TypeHeaderCode
  #include "offline_learners/OfflineIteratingLearner.h"
  %End
  int number_of_iterations;
  int seed;
};

class OfflineIteratingLearner : public OfflineLearner, public Initializable {
 %TypeHeaderCode
 #include "offline_learners/OfflineIteratingLearner.h"
 %End
 public:
  OfflineIteratingLearner(OfflineIteratingLearnerParameters *parameters);
  void iterate();
  void add_simple_updater(ModelSimpleUpdater* model_updater);
  void add_early_simple_updater(ModelSimpleUpdater* model_updater);
  void add_gradient_updater(ModelGradientUpdater* model_updater);
  void set_model(Model* model);
  void set_recommender_data(RecommenderData *recommender_data);
  void set_gradient_computer(GradientComputerPointWise *gradient_computer);
  bool self_test();
 protected:
  bool autocalled_initialize();
};
