#ifndef ABSOLUTE_ERROR_EVALUATOR_H
#define ABSOLUTE_ERROR_EVALUATOR_H

#include "Evaluator.h"
#include "../Model.h"

class AbsoluteErrorEvaluator : public Evaluator {
public:
  double get_loss(RecDat* rec_dat) override;
  void set_model(Model* model){ model_=model; }
  bool self_test(){
    bool ok=true;
    if(model_==NULL)ok=false;
    return ok;
  }
private:
  Model* model_ = NULL;
};
#endif /* ABSOLUTE_ERROR_EVALUATOR_H */
