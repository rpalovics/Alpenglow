#ifndef RANDOM_CHOOSING_COMBINED_MODEL_EXPERT_UPDATER_H
#define RANDOM_CHOOSING_COMBINED_MODEL_EXPERT_UPDATER_H

#include <gtest/gtest_prod.h>
#include <numeric>
#include "DCGEvaluator.h"
#include "AbsoluteErrorEvaluator.h"
#include "RandomChoosingCombinedModel.h"
#include "Evaluator.h"
#include "../ModelUpdater.h"
#include "../../general_interfaces/Initializable.h"
#include "../../general_interfaces/NeedsExperimentEnvironment.h"

using namespace std;

struct RandomChoosingCombinedModelExpertUpdaterParameters {
  double eta = 0.1;
  int top_k = -1;
  string loss_type = ""; //abs, dcg, mrr, other
};

class RandomChoosingCombinedModelExpertUpdater
  : public Updater,
  public WMSUpdater,
  public Initializable,
  public NeedsExperimentEnvironment 
{
public:
  RandomChoosingCombinedModelExpertUpdater(RandomChoosingCombinedModelExpertUpdaterParameters* params)
  : eta_(params->eta),
    top_k_(params->top_k),
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
  void set_wms(WeightedModelStructure* model){model_ = model; }
  void update(RecDat* rec_dat);
  bool self_test(){
    bool ok = dcg_eval_self_test_result_;
    if(model_==NULL){
      ok=false;
      cerr<<"RandomChoosingCombinedModelExpertUpdater::model is not set."<<endl;
    }
    if(loss_type_!="other" && loss_type_!="abs" && loss_type_!="dcg" && loss_type_!="mrr"){
      ok=false;
    }
    if(loss_type_!="other" and loss_type_!="abs" and loss_type_!="dcg"){
      ok=false;
      cerr<<"RandomchoosingCombinedModelExpertUpdater::loss_type=" << loss_type_ << " is not implemented." << endl;
    }
    if(evaluators_.size()!=model_->models_.size()){
      ok=false;
    }
    return ok;
  }

protected:
  bool autocalled_initialize(){
    if(!model_->is_initialized()) return false;
    for(auto& weight:model_->distribution_){
      weight=1.0;
    }
    if(loss_type_=="abs"){
      for(auto model:model_->models_){
        AbsoluteErrorEvaluator* evaluator = new AbsoluteErrorEvaluator;
        evaluator->set_model(model);
        evaluators_.push_back(evaluator);
      }
    }
    if(loss_type_=="dcg"){
      for(auto model:model_->models_){
        if (top_k_<0){
          if (experiment_environment_==NULL) return false;
          top_k_=experiment_environment_->get_top_k();
        }
        DCGEvaluatorParameters params;
        params.top_k = top_k_;
        DCGEvaluator* evaluator = new DCGEvaluator(&params);
        evaluator->set_model(model);
        evaluator->set_experiment_environment(experiment_environment_);
        evaluator->initialize();
        dcg_eval_self_test_result_ &= evaluator->self_test();
        evaluators_.push_back(evaluator);
      }
    }
    //TODO create MRR evaluators
    return true;
  }
private:
  WeightedModelStructure* model_ = NULL;
  vector<Evaluator*> evaluators_;
  double eta_ = 0;
  int top_k_ = -1;
  string loss_type_ = "";
  bool dcg_eval_self_test_result_ = true;
};


#endif /* RANDOM_CHOOSING_COMBINED_MODEL_EXPERT_UPDATER_H */
