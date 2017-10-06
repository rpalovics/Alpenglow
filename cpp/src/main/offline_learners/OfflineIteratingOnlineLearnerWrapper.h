#ifndef OFFLINE_ITERATING_ONLINE_LEARNER_WRAPPER_H_
#define OFFLINE_ITERATING_ONLINE_LEARNER_WRAPPER_H_

#include "OfflineLearner.h"
#include "../utils/Random.h"
#include "../general_interfaces/Updater.h"
#include "../recommender_data/RandomIterator.h"

struct OfflineIteratingOnlineLearnerWrapperParameters {
  int number_of_iterations=10;
  bool shuffle=true;
  int seed=25435429;
};

class OfflineIteratingOnlineLearnerWrapper : public OfflineLearner {
 public:
  OfflineIteratingOnlineLearnerWrapper(OfflineIteratingOnlineLearnerWrapperParameters *parameters) {
    seed_ = parameters->seed;
    number_of_iterations_ = parameters->number_of_iterations;
    shuffle_ = parameters->shuffle;
  }
  void fit(RecommenderData* recommender_data) override;
  void add_early_updater(Updater* model_updater) {
    early_updaters_.push_back(model_updater);
  }
  void add_updater(Updater* model_updater) {
    updaters_.push_back(model_updater);
  }
  void add_iterate_updater(Updater* model_updater) {
    iterate_updaters_.push_back(model_updater);
  }
  bool self_test(){
    bool OK = OfflineLearner::self_test();
    if(seed_==-1 and shuffle_){ OK = false; cerr << "OfflineIteratingOnlineLearnerWrapper::seed is not set but shuffle is set." << endl; }
    if(number_of_iterations_<0 and iterate_updaters_.size()!=0){ OK = false; cerr << "OfflineIteratingOnlineLearnerWrapper::number_of_iterations is not set but iterate updaters are present." << endl; }
    if(iterate_updaters_.size()==0 and updaters_.size()==0 and early_updaters_.size()==0){ OK = false; cerr << "No updaters are added to OfflineIteratingOnlineLearnerWrapper." << endl; }
    return OK;
  }
 private:
  std::vector<Updater*> updaters_;
  std::vector<Updater*> early_updaters_;
  std::vector<Updater*> iterate_updaters_;
  int number_of_iterations_;
  int seed_;
  bool shuffle_;
};

#endif

