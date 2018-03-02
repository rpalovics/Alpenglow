#ifndef EVALUATOR
#define EVALUATOR

#include "../../recommender_data/RecommenderData.h"
class Evaluator {
public:
  virtual double get_score(RecDat*)=0;
  virtual ~Evaluator(){}
};

#endif
