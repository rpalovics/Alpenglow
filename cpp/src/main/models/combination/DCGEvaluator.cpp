#include "DCGEvaluator.h"

double DCGEvaluator::get_score(RecDat* rec_dat){
  int rank = rank_computer_->get_rank(rec_dat);
  return log(2)/log(rank+2); //+2: rank of the first item is 0
}
