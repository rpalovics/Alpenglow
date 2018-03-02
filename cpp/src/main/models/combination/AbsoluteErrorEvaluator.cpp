#include "AbsoluteErrorEvaluator.h"
double AbsoluteErrorEvaluator::get_loss(RecDat* rec_dat){
  double prediction=model_->prediction(rec_dat);
  return abs(prediction-rec_dat->score);
}
