#ifndef DCG_EVALUATOR
#define DCG_EVALUATOR

#include "../Model.h"
#include "Evaluator.h"
#include "../../general_interfaces/Initializable.h"
#include "../../general_interfaces/NeedsExperimentEnvironment.h"

class DCGEvaluator
  : public Evaluator,
    public Initializable,
    public NeedsExperimentEnvironment
{
public:
  double get_score(RecDat* rec_dat) override;
  void set_model(Model* model){ model_=model; }
  void set_experiment_environment(ExperimentEnvironment* experiment_environment) override { experiment_environment_=experiment_environment; }
  void set_items(vector<int>* items){ items_ = items; }
  bool self_test(){
    bool ok=true;
    if (model_==NULL) ok=false;
    if (items_==NULL) ok=false;
    return ok;
  }
    bool autocalled_initialize() override {
      if(items_==NULL){
        items_ = experiment_environment_->get_items();
      }
      return true;
    }
  private:
private:
  ExperimentEnvironment* experiment_environment_ = NULL;
  Model* model_ = NULL;
  vector<int>* items_ = NULL;
};

#endif
