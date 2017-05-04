class OfflineExperiment {
 %TypeHeaderCode
 #include "offline_experiment/OfflineExperiment.h"
 %End
 public:
  OfflineExperiment();
  bool self_test();
  void run();
  void add_offline_learner(OfflineLearner* offline_learner);
  void add_offline_evaluator(OfflineEvaluator* offline_evaluator);
};