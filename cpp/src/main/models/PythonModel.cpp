#include "PythonModel.h"


pair<int, double> PythonModelRankingScoreIterator::get_next(){
  return model_->iterator_get_next_(user_);
};
bool PythonModelRankingScoreIterator::has_next(double upper_bound) {
  return model_->iterator_has_next_(user_, upper_bound);
};
