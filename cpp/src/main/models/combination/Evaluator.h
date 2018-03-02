#ifndef EVALUATOR
#define EVALUATOR

#include "../../recommender_data/RecommenderData.h"
class Evaluator {
public:
  virtual double get_score(RecDat*)=0;
  virtual double get_cumulative_score(RecDat*)=0;
  virtual double get_and_add_score(RecDat*)=0;
};

#endif
