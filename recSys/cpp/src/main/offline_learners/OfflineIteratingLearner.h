#ifndef OFFLINE_FACTOR_MODELL_LEARNER_H_
#define OFFLINE_FACTOR_MODELL_LEARNER_H_
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>
#include "../utils/SpMatrix.h"
#include "OfflineLearner.cpp"
#include "../models/Model.h"
#include "../models/ModelUpdater.h"
#include "../recommender_data/RecommenderData.h"
#include "../recommender_data/RandomIterator.h"
#include "../gradient_computers/GradientComputer.h"
#include "../utils/Random.h"

struct OfflineIteratingLearnerParameters {
  int number_of_iterations;
  int seed;
};

class OfflineIteratingLearner : public OfflineLearner {
 public:
  OfflineIteratingLearner(OfflineIteratingLearnerParameters *parameters) {
    seed_ = parameters->seed;
    number_of_iterations_ = parameters->number_of_iterations;
    initialized_ = false;
    random_iterator_ = NULL;
    recommender_data_ = NULL;
    gradient_computer_ = NULL;
    model_ = NULL;
  }
  void iterate();
  void add_simple_updater(ModelSimpleUpdater* model_updater) {
    simple_updaters_.push_back(model_updater);
  }
  void add_gradient_updater(ModelGradientUpdater* model_updater) {
    gradient_updaters_.push_back(model_updater);
  }
  void set_model(Model* model) {
    model_ = model;
  }
  void set_recommender_data(RecommenderData *recommender_data) {
    recommender_data_ = recommender_data;
    random_iterator_ = new RandomIterator(recommender_data, seed_);
  }
  void set_gradient_computer(GradientComputerPointWise *gradient_computer) {
    gradient_computer_ = gradient_computer;
  }
  bool self_test(){
    bool OK = OfflineLearner::self_test();
    if(seed_==-1){ OK = false; cerr << "OfflineIteratingLearner::seed is not set." << endl; }
    if(number_of_iterations_<0 and gradient_updaters_.size()!=0){ OK = false; cerr << "OfflineIteratingLearner::number_of_iterations is not set but gradient updaters are present." << endl; }
    if(model_==NULL){ OK = false; cerr << "OfflineIteratingLearner::model_ is not set." << endl; }
    if(gradient_updaters_.size()==0 and simple_updaters_.size()==0){ OK = false; cerr << "No updaters are added to OfflineIteratingLearner." << endl; }
    if(gradient_computer_==NULL and gradient_updaters_.size()>0){ OK=false; cerr << "OfflineIteratingLearner::gradient_computer is not set." << endl; }
    if(recommender_data_==NULL){ OK=false; cerr << "OfflineIteratingLearner::recommender_data_ is not set." << endl; }
    if(random_iterator_==NULL){ OK=false; cerr << "Problem with OfflineFactorModel::random_iterator" << endl; }
    return OK;
  }
 private:
  void init();
  std::vector<ModelSimpleUpdater*> simple_updaters_;
  std::vector<ModelGradientUpdater*> gradient_updaters_;
  RandomIterator *random_iterator_;
  RecommenderData *recommender_data_;
  GradientComputerPointWise *gradient_computer_;
  Model *model_;
  int number_of_iterations_;
  int seed_;
  bool initialized_;
};

#endif

