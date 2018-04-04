#ifndef TOPLIST_COMBINATION_MODEL
#define TOPLIST_COMBINATION_MODEL

#include <vector>
#include <gtest/gtest_prod.h>
#include "../Model.h"
#include "../RankingScoreIterator.h"
#include "../../general_interfaces/Initializable.h"
#include "../../general_interfaces/NeedsExperimentEnvironment.h"

using namespace std;

class ToplistCombinationModel
 : public Model,
   virtual public RankingScoreIteratorProvider,
   public Initializable,
   public NeedsExperimentEnvironment
{
public:
  void add_model(Model* model){
    models_.push_back(model);
    rsip_models_.push_back(dynamic_cast<RankingScoreIteratorProvider*>(model));
  }
  void set_experiment_environment(ExperimentEnvironment* experiment_environment){ experiment_environment_ = experiment_environment; }
  bool self_test(){
    bool ok = Model::self_test();
    if(models_.size()==0) ok=false;
    return ok;
  }
  void add(RecDat* rec_dat) override;
  double prediction(RecDat* rec_dat) override;
  void write(ostream& file) override;
  void read(istream& file) override;
  RankingScoreIterator* get_ranking_score_iterator(int user) override;
protected:
  bool autocalled_initialize(){
    random_=experiment_environment_->get_random();
    top_k_=experiment_environment_->get_top_k();
    distribution_.clear(); //should not be called twice, but...
    distribution_.resize(models_.size(),1.0/models_.size());
    return true;
  }
private:
  void generate_random_values_for_toplists();
  vector<int> random_model_indices_;
  void compute_score_map();
  map<int,double> scores_;
  void compute_last_occ_of_models();
  vector<int> last_occ_of_models_;
  bool test_top_k();
  void compute_toplists();
  void merge_toplists();

  vector<Model*> models_;
  vector<RankingScoreIteratorProvider*> rsip_models_; //TODO ez kell?
  vector<double> distribution_;
  //cache
  double last_timestamp_ = -1;
  int last_user_ = -1;
  int last_id_ = -1;
  Random* random_ = NULL;
  int top_k_ = -1;
  ExperimentEnvironment* experiment_environment_ = NULL;
  FRIEND_TEST(TestToplistCombinationModel, generate_random_values_for_toplists);
  FRIEND_TEST(TestToplistCombinationModel, compute_last_occ_of_models);
  //friend class RandomChoosingCombinedModelExpertUpdater;
};

#endif
