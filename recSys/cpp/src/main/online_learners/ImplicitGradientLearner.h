#ifndef IGLEARNER
#define IGLEARNER

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
#include "OnlineLearner.cpp"
#include "../gradient_computers/GradientComputer.h"
#include "../models/Model.h"
#include "../recommender_data/RecommenderData.h"
#include <iomanip>
#include "../negative_sample_generators/NegativeSampleGenerator.h"
#include "../general_interfaces/INeedExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"

class ImplicitGradientLearner : public OnlineLearner, public Initializable, public INeedExperimentEnvironment{
  public:
    ImplicitGradientLearner(){
      model_=NULL;
    }
    ~ImplicitGradientLearner(){};
    void learn(RecDat* rec_dat) override;

    void set_train_matrix(SpMatrix* train_matrix){train_matrix_=train_matrix;}
    void set_gradient_computer(GradientComputer* gradient_computer){gradient_computer_=gradient_computer;}
    void set_negative_sample_generator(NegativeSampleGenerator* negative_sample_generator){ negative_sample_generator_ = negative_sample_generator; }
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override {
      experiment_environment_=experiment_environment;
    }
    bool init() override {
      if(train_matrix_==NULL){ train_matrix_=experiment_environment_->get_train_matrix(); }
      lookback_=experiment_environment_->is_lookback();
      return true;
    }
    bool self_test(){
      bool ok = OnlineLearner::self_test();
      if(negative_sample_generator_==NULL){
        ok = false;
        cerr << "ImplicitGradientLearner::negative_sample_generator is not set." << endl;
      }
      if(train_matrix_==NULL){
        ok = false;
        cerr << "ImplicitGradientLearner::train_matrix is not set." << endl;
      }
      if(model_gradient_updaters_.size()!=0 and gradient_computer_==NULL){
        ok = false;
        cerr << "ImplicitGradientLearner::gradient_computer is not set." << endl;
      }
      return ok;
    }
  protected:
    ExperimentEnvironment* experiment_environment_ = NULL;
    SpMatrix* train_matrix_ = NULL;
    GradientComputer* gradient_computer_ = NULL;
    NegativeSampleGenerator* negative_sample_generator_ = NULL;
    bool lookback_;
};
#endif
