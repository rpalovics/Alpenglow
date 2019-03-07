#ifndef OBJECTIVE_B_P_R_H
#define OBJECTIVE_B_P_R_H

#include "Objective.h"

using namespace std;

class ObjectiveBPR : public ObjectivePairWise{
  public:
    pair <double,double> get_gradient(RecPred* rec_pred1, RecPred* rec_pred2);
};

#endif /* OBJECTIVE_B_P_R_H */
