#ifndef TOPLIST_COMBINATION_MODEL_RANKING_SCORE_ITERATOR_H
#define TOPLIST_COMBINATION_MODEL_RANKING_SCORE_ITERATOR_H

//SIP_AUTOCONVERT

#include <vector>
#include <set>
#include <stdexcept>
#include <gtest/gtest_prod.h>
#include "WeightedModelStructure.h"
#include "../Model.h"
#include "../RankingScoreIterator.h"
#include "../../general_interfaces/Initializable.h"
#include "../../general_interfaces/NeedsExperimentEnvironment.h"
#include "../../utils/RankComputer.h"
#include "../../utils/ToplistCreator.h"
#include "../../filters/DummyModelFilter.h"

using namespace std;

class ToplistCombinationModelRankingScoreIterator : public RankingScoreIterator{
public:
  bool has_next(double bound) override;
  pair<int, double> get_next() override;
  void set_up(vector<pair<int,double>> toplist){ counter_ = 0; current_scores_ = toplist; }
  void reinit(){counter_=0;}
  int unique_items_num(){ throw runtime_error("Not implemented. Should not be called by RankComputer as all scores are nonnegative."); }
private:
  void clear(){counter_=0;current_scores_.clear();}
  vector<pair<int,double>> current_scores_;
  int counter_;
};

struct ToplistCombinationModelParameters{
  int seed = 745578;
  int top_k = -1;
};
class ToplistCombinationModel
 : public Model,
   virtual public RankingScoreIteratorProvider,
   public Initializable,
   public NeedsExperimentEnvironment
{
public:
  ToplistCombinationModel(ToplistCombinationModelParameters* params){
    random_.set(params->seed);
    seed_ = params->seed;
  }
  void add_model(Model* model){
    wms_.models_.push_back(model);
  }
  bool self_test(){
    bool ok = Model::self_test() && random_.self_test();
    if(wms_.models_.size()==0) ok=false;
    for(auto rank_computer : rank_computers_) ok &= rank_computer->self_test();
    for(auto toplist_creator : toplist_creators_) ok &= toplist_creator->self_test(); 
    return ok;
  }
  void add(RecDat* rec_dat) override;
  double prediction(RecDat* rec_dat) override;
  void write(ostream& file) override;
  void read(istream& file) override;
  RankingScoreIterator* get_ranking_score_iterator(int user) override;
  ~ToplistCombinationModel(){
    for(auto rank_computer: rank_computers_){ delete rank_computer; }
    for(auto toplist_creator: toplist_creators_){ delete toplist_creator; }
    rank_computers_.clear();
  }
  void inject_wms_into(WMSUpdater* object){ object->set_wms(&wms_); }
protected:
  bool autocalled_initialize() override {
    if(top_k_==-1){
      if(experiment_environment_==NULL) return false;
      top_k_=experiment_environment_->get_top_k();
    }
    wms_.distribution_.clear(); //should not be called twice, but...
    wms_.distribution_.resize(wms_.models_.size(),1.0/wms_.models_.size());
    dummy_model_filter_.set_experiment_environment(experiment_environment_);
    bool ok = true;
    ok &= dummy_model_filter_.initialize();
    for(auto model : wms_.models_){
      RankComputerParameters rank_computer_params;
      rank_computer_params.top_k=top_k_;
      rank_computer_params.random_seed=seed_+1;
      RankComputer* rank_computer = new RankComputer(&rank_computer_params);
      rank_computers_.push_back(rank_computer);
      rank_computer->set_experiment_environment(experiment_environment_);
      rank_computer->set_model(model);
      ok &= rank_computer->initialize();
      ToplistCreatorPersonalizedParameters toplist_computer_params;
      ToplistCreatorPersonalized* toplist_creator = new ToplistCreatorPersonalized(&toplist_computer_params);
      toplist_creators_.push_back(toplist_creator);
      toplist_creator->set_experiment_environment(experiment_environment_);
      toplist_creator->set_model(model);
      toplist_creator->set_filter(&dummy_model_filter_);
      ok &= toplist_creator->initialize();
    }
    wms_.initialized_=true;
    return ok;
  }
private:
  WeightedModelStructure wms_;
  ToplistCombinationModelRankingScoreIterator rsi_;
  void recompute_predictions(RecDat* rec_dat);
  bool random_values_generated_ = false;
  void generate_random_values_for_toplists();
  vector<int> random_model_indices_;
  void compute_score_map();
  map<int,double> scores_;
  void compute_last_occ_of_models();
  vector<int> last_occ_of_models_;
  bool test_top_k(RecDat*);
  void compute_toplists(RecDat* rec_dat);
  vector<vector<pair<int,double>>> toplists_; //models->toplist->(item,score)
  void merge_toplists();
  vector<pair<int,double>> toplist_;

  //vector<Model*> models_;
  vector<RankComputer*> rank_computers_;
  vector<ToplistCreatorPersonalized*> toplist_creators_;
  DummyModelFilter dummy_model_filter_;
  //vector<double> distribution_;
  //cache
  double last_timestamp_ = -1;
  int last_user_ = -1;
  int last_id_ = -1;
  Random random_;
  int seed_ = -1;
  int top_k_ = -1;
  FRIEND_TEST(TestToplistCombinationModel, generate_random_values_for_toplists);
  FRIEND_TEST(TestToplistCombinationModel, compute_last_occ_of_models);
  FRIEND_TEST(TestToplistCombinationModel, test_top_k);
  FRIEND_TEST(TestToplistCombinationModel, compute_toplists);
  FRIEND_TEST(TestToplistCombinationModel, merge_toplists);
  FRIEND_TEST(TestToplistCombinationModel, prediction);
  //friend class RandomChoosingCombinedModelExpertUpdater;
};

#endif /* TOPLIST_COMBINATION_MODEL_RANKING_SCORE_ITERATOR_H */
