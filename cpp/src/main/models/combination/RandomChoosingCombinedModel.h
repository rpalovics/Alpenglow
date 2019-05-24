#ifndef RANDOM_CHOOSING_COMBINED_MODEL_H
#define RANDOM_CHOOSING_COMBINED_MODEL_H

//SIP_AUTOCONVERT

#include <vector>
#include <gtest/gtest_prod.h>
#include "WeightedModelStructure.h"
#include "../Model.h"
#include "../RankingScoreIterator.h"
#include "../../general_interfaces/Initializable.h"
#include "../../utils/Random.h"

using namespace std;

struct RandomChoosingCombinedModelParameters{
  int seed = 745578;
};
class RandomChoosingCombinedModel
 : public Model,
   virtual public RankingScoreIteratorProvider,
   public Initializable
{
public:
  RandomChoosingCombinedModel(RandomChoosingCombinedModelParameters* params){
    random_.set(params->seed);
  }
  void add_model(Model* model){
    wms_.models_.push_back(model);
    rsip_models_.push_back(dynamic_cast<RankingScoreIteratorProvider*>(model));
  }
  bool self_test(){
    bool ok = Model::self_test() && random_.self_test();
    ok&=random_.self_test();
    if(wms_.models_.size()==0) ok=false;
    return ok;
  }
  void add(RecDat* rec_dat) override;
  double prediction(RecDat* rec_dat) override;
  void write(ostream& file) override;
  void read(istream& file) override;
  RankingScoreIterator* get_ranking_score_iterator(int user) override;
  void inject_wms_into(WMSUpdater* object){ object->set_wms(&wms_); }
protected:
  bool autocalled_initialize() override {
    wms_.distribution_.clear(); //should not be called twice, but...
    wms_.distribution_.resize(wms_.models_.size(),1.0/wms_.models_.size());
    active_model_id_=0;
    active_model_=wms_.models_[0];
    wms_.initialized_ = true;
    return true;
  }
private:
  WeightedModelStructure wms_;
  //vector<Model*> models_;
  vector<RankingScoreIteratorProvider*> rsip_models_;
  //vector<double> distribution_;
  int active_model_id_ = -1;
  Model* active_model_=NULL;
  double last_timestamp_ = -1;
  int last_user_ = -1;
  int last_id_ = -1;
  Random random_;
  FRIEND_TEST(TestRandomChoosingCombinedModel, prediction_distribution);
  friend class RandomChoosingCombinedModelExpertUpdater;
};


#endif /* RANDOM_CHOOSING_COMBINED_MODEL_H */
