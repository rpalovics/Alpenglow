#ifndef DCG_EVALUATOR
#define DCG_EVALUATOR

#include "../Model.h"
#include "Evaluator.h"
class DCGEvaluator : public Evaluator {
  double get_score(RecDat* rec_dat) override;
  void set_model(Model* model){ model_=model; }
  bool self_test(){
    bool ok=true;
    if(model_==NULL)ok=false;
    return ok;
  }
private:
  Model* model_ = NULL;
};

#endif
