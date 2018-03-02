#ifndef EVALUATOR
#define EVALUATOR

#include "../../recommender_data/RecommenderData.h"
class Evaluator {
public:
  virtual double get_score(RecDat* rec_dat){ return -1*get_loss(rec_dat); }
  virtual double get_loss(RecDat* rec_dat){ return -1*get_score(rec_dat); }
  virtual ~Evaluator(){}
};

#endif
