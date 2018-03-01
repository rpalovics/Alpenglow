#ifndef OBJECTIVE_BPR
#define OBJECTIVE_BPR

#include "Objective.h"

using namespace std;

class ObjectiveBPR : public ObjectivePairWise{
  public:
    pair <double,double> get_gradient(RecPred* rec_pred1, RecPred* rec_pred2);
};

#endif
