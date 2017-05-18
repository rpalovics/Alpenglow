class ObjectiveMSE : public ObjectivePointWise{
  %TypeHeaderCode
  #include "objectives/ObjectiveMSE.h"
  %End
  public:
    ObjectiveMSE();
    ~ObjectiveMSE();
    double get_gradient (RecPred  * rec_pred);
};
