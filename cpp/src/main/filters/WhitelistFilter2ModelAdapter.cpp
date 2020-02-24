#include "WhitelistFilter2ModelAdapter.h"

//WhitelistFilterRankingScoreIterator
bool WhitelistFilterRankingScoreIterator::has_next(double lower_bound) {
  return counter_<(int)whitelist_.size();
}
pair<int,double> WhitelistFilterRankingScoreIterator::get_next() {
  pair<int,double> next;
  int item = whitelist_.at(counter_);
  next.first = item;
  current_sample_.item = item;
  next.second = model_->prediction(&current_sample_);
  counter_++;
  return next;
}
int WhitelistFilterRankingScoreIterator::unique_items_num() {
  return whitelist_.size();
}
void WhitelistFilterRankingScoreIterator::reset(int user, vector<int> whitelist) {
  counter_ = 0;
  current_sample_.user = user;
  whitelist_ = whitelist;
}

//WhitelistFilter2ModelAdapter
double WhitelistFilter2ModelAdapter::prediction(RecDat* rec_dat) {
  if (whitelist_filter_->active(rec_dat)){
    return model_->prediction(rec_dat);
  } else {
    return 0;
  }
}
RankingScoreIterator* WhitelistFilter2ModelAdapter::get_ranking_score_iterator(int user) {
  vector<int> whitelist = whitelist_filter_->get_whitelist(user);
  rsi_.reset(user,whitelist);
  return &rsi_;
}
