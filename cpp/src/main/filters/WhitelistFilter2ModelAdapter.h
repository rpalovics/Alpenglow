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
  bool has_next(double lower_bound) override;
  pair<int,double> get_next() override;
  int unique_items_num() override;
  void reset(int user, vector<int> whitelist);

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
  double prediction(RecDat* rec_dat) override;
  RankingScoreIterator* get_ranking_score_iterator(int user) override;

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
