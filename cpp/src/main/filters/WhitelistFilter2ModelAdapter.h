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
/**
Adapter class to filter the output of a model.

By chaining the adapter in front of a model, we can filter the output of the model, allowing only items on the whitelist filter to the toplist.

Note that as the currently implemented whitelists contain only a few elements, the adapter interface algorithm is optimized for short whitelists.  We ignore the RSI of the model.
*/
public:
  double prediction(RecDat* rec_dat) override;
  /**
  prediction(RecDat*)

  Returns filtered prediction value.  The score of the items that are not on the whitelist is set to 0.
  Overrides method inherited from :py:meth:`alpenglow.cpp.Model.prediction`, see also documentation there.

  Parameters
  ----------
  rec_dat : RecDat*
      The sample we query the prediction for.

  Returns
  -------
  double
      The prediction score, modified based on the filter.  If the item is not on the whitelist, the returned score is 0, otherwise the score returned by the model.
  */
  RankingScoreIterator* get_ranking_score_iterator(int user) override;

  void set_whitelist_filter(WhitelistFilter* whitelist_filter){
    whitelist_filter_ = whitelist_filter;
  }
  /**
  set_whitelist_filter(WhitelistFilter* whitelist_filter)

  Sets whitelist filter.

  Parameters
  ----------
  whitelist_filter : WhitelistFilter*
      The whitelist filter we use for filtering the output of the model.
  */
  void set_model(Model* model){
    model_ = model;
    rsi_.set_model(model);
  }
  /**
  set_model(Model* model)

  Sets model whose output is filtered.

  Parameters
  ----------
  model : Model*
      The model whose output is filtered.
  */
  bool self_test(){
    bool ok = Model::self_test(); // && RankingScoreIteratorProvider::self_test()
    if (!rsi_.self_test()) ok = false;
    if (model_==NULL) ok = false;
    if (whitelist_filter_==NULL) ok = false;
    return ok;
  }
  /**
  Tests whether the model and the whitelist filter is set.

  Returns
  -------
  bool
      Whether all necessary objects are set.
  */
private:
  WhitelistFilter* whitelist_filter_ = NULL;
  Model* model_ = NULL;
  WhitelistFilterRankingScoreIterator rsi_;
};

#endif /* WHITE_LIST_FILTER_2_MODEL_ADAPTER_H */
