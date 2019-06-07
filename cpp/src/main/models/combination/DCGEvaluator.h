#ifndef D_C_G_EVALUATOR_H
#define D_C_G_EVALUATOR_H

#include "../Model.h"
#include "Evaluator.h"
#include "../../general_interfaces/Initializable.h"
#include "../../general_interfaces/NeedsExperimentEnvironment.h"
#include "../../utils/RankComputer.h"


//TODO should be compatible with the offline environment
struct DCGEvaluatorParameters {
  int top_k = 100;
};
class DCGEvaluator
  : public Evaluator,
    public Initializable,
    public NeedsExperimentEnvironment
{
public:
  DCGEvaluator(DCGEvaluatorParameters* params)
  :top_k_(params->top_k)
  {
    RankComputerParameters rank_computer_params;
    rank_computer_params.top_k=top_k_;
    rank_computer_params.random_seed=19263435; //TODO get rid of random seed here, RankComputer should use the common random object
    rank_computer_=new RankComputer(&rank_computer_params);
  }
  ~DCGEvaluator(){ delete rank_computer_; }
  double get_score(RecDat* rec_dat) override;
  void set_model(Model* model){
    rank_computer_->set_model(model);
  }
  bool self_test(){
    bool ok=Evaluator::self_test() && rank_computer_->self_test();
    return ok;
  }
  bool autocalled_initialize() override {
    rank_computer_->set_experiment_environment(experiment_environment_);
    if(!rank_computer_->is_initialized()) return rank_computer_->initialize();
    else return true;
  }
private:
  int top_k_ = 100;
  RankComputer* rank_computer_ = NULL;
};

#endif /* D_C_G_EVALUATOR_H */
