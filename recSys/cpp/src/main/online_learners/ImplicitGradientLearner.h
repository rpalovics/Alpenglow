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

class ImplicitGradientLearner : public OnlineLearner{
  public:
    ImplicitGradientLearner(){
      model_=NULL;
      negative_sample_generator_=NULL;
      train_matrix_=NULL;
      gradient_computer_=NULL;
    }
    ~ImplicitGradientLearner(){};
    void learn(RecDat* rec_dat) override;

    void set_train_matrix(SpMatrix* train_matrix){train_matrix_=train_matrix;}
    void set_gradient_computer(GradientComputer* gradient_computer){gradient_computer_=gradient_computer;}
    void set_negative_sample_generator(NegativeSampleGenerator* negative_sample_generator){ negative_sample_generator_ = negative_sample_generator; }
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
    SpMatrix* train_matrix_;
    GradientComputer* gradient_computer_;
    NegativeSampleGenerator* negative_sample_generator_;
};
#endif
