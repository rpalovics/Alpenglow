struct OfflineIteratingOnlineLearnerWrapperParameters {
  %TypeHeaderCode
  #include "offline_learners/OfflineIteratingOnlineLearnerWrapper.h"
  %End
  int number_of_iterations;
  bool shuffle;
  int seed;
};

class OfflineIteratingOnlineLearnerWrapper : public OfflineLearner {
  %TypeHeaderCode
  #include "offline_learners/OfflineIteratingOnlineLearnerWrapper.h"
  %End
 public:
  OfflineIteratingOnlineLearnerWrapper(OfflineIteratingOnlineLearnerWrapperParameters *parameters);
  void iterate();
  void fit(RecommenderData* recommender_data);
  void set_recommender_data(RecommenderData* recommender_data);
  void add_early_updater(Updater* model_updater);
  void add_updater(Updater* model_updater);
  void add_iterate_updater(Updater* model_updater);
  bool self_test();
};
