
// class WhitelistFilterRankingScoreIterator : public RankingScoreIterator {
//  %TypeHeaderCode
//  #include "filters/WhitelistFilter2ModelAdapter.h"
//  %End
//public:
//  WhitelistFilterRankingScoreIterator();
//  bool has_next(double lower_bound);
//  pair<int,double> get_next();
//  int unique_items_num();
//  void reset(int user, std::vector<int> whitelist);
// 
//  void set_model(Model* model);
//  bool self_test();
//};

class WhitelistFilter2ModelAdapter
  : public Model
//  , public RankingScoreIteratorProvider
{
  %TypeHeaderCode
  #include "filters/WhitelistFilter2ModelAdapter.h"
  %End
  %Docstring
Adapter class to filter the output of a model.

By chaining the adapter in front of a model, we can filter the output of the model, allowing only items on the whitelist filter to the toplist.

Note that as the currently implemented whitelists contain only a few elements, the adapter interface algorithm is optimized for short whitelists.  We ignore the RSI of the model.
  %End
public:
  double prediction(RecDat* rec_dat);
  %Docstring
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
  %End
//  RankingScoreIterator* get_ranking_score_iterator(int user);

  void set_whitelist_filter(WhitelistFilter* whitelist_filter);
  %Docstring
  set_whitelist_filter(WhitelistFilter* whitelist_filter)

  Sets whitelist filter.

  Parameters
  ----------
  whitelist_filter : WhitelistFilter*
      The whitelist filter we use for filtering the output of the model.
  %End
  void set_model(Model* model);
  %Docstring
  set_model(Model* model)

  Sets model whose output is filtered.

  Parameters
  ----------
  model : Model*
      The model whose output is filtered.
  %End
  bool self_test();
  %Docstring
  Tests whether the model and the whitelist filter is set.

  Returns
  -------
  bool
      Whether all necessary objects are set.
  %End
};

