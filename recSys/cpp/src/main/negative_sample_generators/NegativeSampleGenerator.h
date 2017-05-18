#ifndef NEGATIVESAMPLEGENERATOR
#define NEGATIVESAMPLEGENERATOR

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
//#include "../ranking/Ranking.h"
#include "../recommender_data/RecommenderData.h"
#include "../utils/Random.h"


class NegativeSampleGenerator{
  public:
    NegativeSampleGenerator(){};
    virtual ~NegativeSampleGenerator(){};
    virtual vector<int>* generate(RecDat* recDat)=0; //deprecated, should not be called from other classes
    virtual vector<RecDat>* get_implicit_train_data(RecDat* positive_sample);
    bool self_test(){
      return true;
    }
  protected:
    vector<int> samples; //TODO rename samples_
    vector<RecDat> implicit_train_data_;
};


#endif
