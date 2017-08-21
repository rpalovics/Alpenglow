#ifndef OFFLINE_EXPERIMENT_H_
#define OFFLINE_EXPERIMENT_H_
#include <iostream>
#include "../offline_learners/OfflineLearner.h"
#include "../offline_evaluators/OfflineEvaluator.h"

class OfflineExperiment {
 public:
  OfflineExperiment(){}
  bool self_test() {
    bool ok = true;
    if (  0 == offline_learners_.size() ){
      std::cerr << "w arning: OfflineExperiment:: offline learners is not set" << std::endl;
    }
    if (  0 == offline_evaluators_.size() ){
      std::cerr << "w arning: OfflineExperiment:: offline evaluator is not set" << std::endl;
    }
    return ok;
  }
  void run();
  void add_offline_learner(OfflineLearner* offline_learner){ offline_learners_.push_back(offline_learner); }
  void add_offline_evaluator(OfflineEvaluator* offline_evaluator){ offline_evaluators_.push_back(offline_evaluator); }
 private:
  vector<OfflineLearner*> offline_learners_;
  vector<OfflineEvaluator*> offline_evaluators_;
};

#endif
