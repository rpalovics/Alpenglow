struct OfflineIteratingImplicitLearnerParameters {
  %TypeHeaderCode
  #include "offline_learners/OfflineIteratingImplicitLearner.h"
  %End
  int number_of_iterations;
  int seed;
};

class OfflineIteratingImplicitLearner : public OfflineLearner {
 %TypeHeaderCode
 #include "offline_learners/OfflineIteratingImplicitLearner.h"
 %End
 public:
  OfflineIteratingImplicitLearner(OfflineIteratingImplicitLearnerParameters *parameters);
  void iterate();
  void add_simple_updater(ModelSimpleUpdater* model_updater);
  void add_early_simple_updater(ModelSimpleUpdater* model_updater);
  void add_gradient_updater(ModelGradientUpdater* model_updater);
  void set_model(Model* model);
  void set_recommender_data(RecommenderData *recommender_data);
  void set_gradient_computer(GradientComputer *gradient_computer);
  void set_negative_sample_generator(NegativeSampleGenerator *negative_sample_generator);
  bool self_test();
};