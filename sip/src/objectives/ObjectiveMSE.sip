
class ObjectiveMSE : public ObjectivePointWise{
  %TypeHeaderCode
  #include "objectives/ObjectiveMSE.h"
  %End
  public:
    double get_gradient(RecPred* rec_pred);
};

