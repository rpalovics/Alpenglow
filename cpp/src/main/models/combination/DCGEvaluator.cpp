#include "DCGEvaluator.h"

double DCGEvaluator::get_score(RecDat* rec_dat){
  return rank_computer_->get_rank(rec_dat);
}
