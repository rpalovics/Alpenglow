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
#include "../gradient_computers/GradientComputer.h"
#include "../models/Model.h"
#include "../models/ModelUpdater.h"
#include "../models/GroupUpdater.h"
#include "../recommender_data/RecommenderData.h"
#include <iomanip>
#include "../negative_sample_generators/NegativeSampleGenerator.h"
#include "../general_interfaces/INeedExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"
#include "../general_interfaces/Updater.h"

class ImplicitGradientLearner : public Updater, public Initializable, public INeedExperimentEnvironment{
  public:
    ImplicitGradientLearner(){
      model_=NULL;
    }
    ~ImplicitGradientLearner(){};
    void update(RecDat* rec_dat) override;

    void set_train_matrix(SpMatrix* train_matrix){train_matrix_=train_matrix;}
    void set_gradient_computer(GradientComputer* gradient_computer){gradient_computer_=gradient_computer;}
    void set_negative_sample_generator(NegativeSampleGenerator* negative_sample_generator){ negative_sample_generator_ = negative_sample_generator; }
    void set_experiment_environment(ExperimentEnvironment* experiment_environment) override {
      experiment_environment_=experiment_environment;
    }
    void add_simple_updater(ModelSimpleUpdater* model_updater) {
      model_simple_updaters_.push_back(model_updater);
    }
    void add_gradient_updater(ModelGradientUpdater* model_updater) {
      model_gradient_updaters_.push_back(model_updater);
    }
    void add_multi_updater(ModelMultiUpdater* model_updater) {
      model_multi_updaters_.push_back(model_updater);
    }
    virtual void set_model(Model* model){model_=model;}
    bool self_test(){
      bool ok = Updater::self_test();
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
    bool autocalled_initialize() override {
      if(train_matrix_==NULL){ train_matrix_=experiment_environment_->get_train_matrix(); }
      exclude_known_=experiment_environment_->do_exclude_known();
      return true;
    }
    ExperimentEnvironment* experiment_environment_ = NULL;
    SpMatrix* train_matrix_ = NULL;
    GradientComputer* gradient_computer_ = NULL;
    NegativeSampleGenerator* negative_sample_generator_ = NULL;
    vector<ModelSimpleUpdater*> model_simple_updaters_;
    vector<ModelGradientUpdater*> model_gradient_updaters_;
    vector<ModelMultiUpdater*> model_multi_updaters_;
    Model* model_;
    bool exclude_known_;
};
#endif
