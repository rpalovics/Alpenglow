#ifndef RECOMMENDER
#define RECOMMENDER

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>
#include "../utils/SpMatrix.h"
#include "../online_learners/OnlineLearner.h"
#include "../models/Model.h"
#include "../recommender_data/RecommenderData.h"

using namespace std;

typedef vector < pair <int,double> > RecVector;
typedef map < int,double > RecMap;

class OnlineRecommender{
  public:
    OnlineRecommender(){
      model_=NULL;
      learner_=NULL;
    }
    ~OnlineRecommender(){}
    void set_model(Model* model){model_ = model;}
    void set_learner(OnlineLearner* learner){learner_ = learner;}
    void learn(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
    bool self_test(){
      bool ok=true;
      if(model_==NULL){ok=false;cerr<<"OnlineRecommender::model_ is not set." << endl;}
      if(learner_==NULL){ok=false;cerr<<"OnlineRecommender::learner_ is not set." << endl;}
      return ok;
    }
  protected:
    Model* model_;
    OnlineLearner* learner_;
};

#endif
