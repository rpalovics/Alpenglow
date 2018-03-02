#ifndef RANDOM_CHOOSING_COMBINED_MODEL_EXPERT_UPDATER
#define RANDOM_CHOOSING_COMBINED_MODEL_EXPERT_UPDATER

#include "RandomChoosingCombinedModel.h"
#include "Evaluator.h"
#include "../ModelUpdater.h"
#include <gtest/gtest_prod.h>

struct RandomChoosingCombinedModelExpertUpdaterParameters{
  double eta = 0.1;
  string loss_type = ""; //abs, dcg, mrr, other
};

class RandomChoosingCombinedModelExpertUpdater
  : public Updater,
  public Initializable
{
public:
  RandomChoosingCombinedModelExpertUpdater(RandomChoosingCombinedModelExpertUpdaterParameters* params)
  : eta_(params->eta),
    loss_type_(params->loss_type)
  {}
  ~RandomChoosingCombinedModelExpertUpdater(){
    if(loss_type_!="other"){
      for(auto evaluator : evaluators_){ delete evaluator; }
    }
  }
  void set_evaluators(vector<Evaluator*> evaluators){
    if(loss_type_!="other"){
      cerr << "RandomChoosingCombinedModelExpertUpdater:Set loss_type=other to set evaluators." << endl;
      return;
    }
    evaluators_ = evaluators;
  }
  void set_model(RandomChoosingCombinedModel* model){model_ = model; }
  void update(RecDat* rec_dat);
  bool self_test(){
    bool ok = true;
    if(model_==NULL){
      ok=false;
      cerr<<"RandomChoosingCombinedModelExpertUpdater::model is not set."<<endl;
    }
    if(loss_type_!="other" && loss_type_!="abs" && loss_type_!="dcg" && loss_type_!="mrr"){
      ok=false;
    }
    if(loss_type_!="other"){
      ok=false;
      cerr<<"RandomchoosingCombinedModelExpertUpdater::loss_type=" << loss_type_ << " is not implemented." << endl;
    }
    //TODO test if evaluators.size=models.size
    return ok;
  }

protected:
  bool autocalled_initialize(){
    //TODO create evaluators
    //for(auto model:model_->models){
    //  RankingScoreIteratorProvider* rsi = dynamic_cast<RankingScoreIteratorProvider*>(model);
    //  iterator_models_.push_back(rsi);
    //}
    return true;
  }
private:
  //vector<RankingScoreIteratorProvider*> iterator_models_;
  RandomChoosingCombinedModel* model_ = NULL;
  vector<Evaluator*> evaluators_;
  double eta_ = 0;
  string loss_type_ = "";
};


#endif
