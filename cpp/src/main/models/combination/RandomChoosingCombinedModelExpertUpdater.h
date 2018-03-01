#ifndef RANDOM_CHOOSING_COMBINED_MODEL_EXPERT_UPDATER
#define RANDOM_CHOOSING_COMBINED_MODEL_EXPERT_UPDATER

#include "RandomChoosingCombinedModel.h"
#include "../ModelUpdater.h"
#include <gtest/gtest_prod.h>

struct RandomChoosingCombinedModelExpertUpdaterParameters{
  double eta = 0.1;
};

class RandomChoosingCombinedModelExpertUpdater
  : public Updater,
  public Initializable
{
public:
  RandomChoosingCombinedModelExpertUpdater(RandomChoosingCombinedModelExpertUpdaterParameters* params)
  : eta_(params->eta)
  {}
  void set_model(RandomChoosingCombinedModel* model){model_ = model; }
  void update(RecDat* rec_dat);
  bool self_test(){
    bool ok = true;
    if(model_==NULL){
      ok=false;
      cerr<<"RandomChoosingCombinedModelExpertUpdater::model is not set."<<endl;
    }
    return ok;
  }

protected:
  bool autocalled_initialize(){
    //for(auto model:model_->models){
    //  RankingScoreIteratorProvider* rsi = dynamic_cast<RankingScoreIteratorProvider*>(model);
    //  iterator_models_.push_back(rsi);
    //}
    return true;
  }
private:
  //vector<RankingScoreIteratorProvider*> iterator_models_;
  RandomChoosingCombinedModel* model_ = NULL;
  double eta_ = 0;
};


#endif
