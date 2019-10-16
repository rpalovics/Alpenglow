#ifndef WHITE_LIST_FILTER_2_MODEL_ADAPTER
#define WHITE_LIST_FILTER_2_MODEL_ADAPTER

#include <vector>
#include "../recommender_data/RecommenderData.h"
#include "../models/Model.h"
#include "../models/RankingScoreIterator.h"
#include "WhitelistFilter.h"

//SIP_AUTOCONVERT

class WhitelistFilterRankingScoreIterator : public RankingScoreIterator {
public:
  WhitelistFilterRankingScoreIterator(){
    current_sample_.id = -1;
    current_sample_.time = 0;
    current_sample_.score = 1;
  }
  bool has_next(double lower_bound) override {
    return counter_<whitelist_.size();
  }
  pair<int,double> get_next() override {
    pair<int,double> next;
    int item = whitelist_.at(counter_);
    next.first = item;
    current_sample_.item = item;
    next.second = model_->prediction(&current_sample_);
    counter_++;
    return next;
  }
  int unique_items_num() override { return whitelist_.size(); }
  void reset(int user, vector<int> whitelist){
    counter_ = 0;
    current_sample_.user = user;
    whitelist_ = whitelist;
  }
  void set_model(Model* model){ model_ = model; }
  bool self_test(){
    bool ok = true; // RankingScoreIterator::self_test();
    if (model_==NULL) ok=false;
    return ok;
  }
private:
  int counter_ = 0;
  RecDat current_sample_; //TODO RSIP:get_rsi(RecDat*)
  vector<int> whitelist_;
  Model* model_ = NULL;
};

class WhitelistFilter2ModelAdapter
  : public Model
  , public RankingScoreIteratorProvider
{
public:
  double prediction(RecDat* rec_dat) override {
    if (whitelist_filter_->active(rec_dat)){
      return model_->prediction(rec_dat);
    } else {
      return 0;
    }
  }
  RankingScoreIterator* get_ranking_score_iterator(int user) override {
    vector<int> whitelist = whitelist_filter_->get_whitelist(user);
    rsi_.reset(user,whitelist);
    return &rsi_;
  }
  void set_whitelist_filter(WhitelistFilter* whitelist_filter){
    whitelist_filter_ = whitelist_filter;
  }
  void set_model(Model* model){
    model_ = model;
    rsi_.set_model(model);
  }
  bool self_test(){
    bool ok = Model::self_test(); // && RankingScoreIteratorProvider::self_test()
    if (!rsi_.self_test()) ok = false;
    return ok;
  }
private:
  WhitelistFilter* whitelist_filter_ = NULL;
  Model* model_ = NULL;
  WhitelistFilterRankingScoreIterator rsi_;
};

#endif /* WHITE_LIST_FILTER_2_MODEL_ADAPTER_H */
