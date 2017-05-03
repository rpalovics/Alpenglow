class ObjectiveMSE : public ObjectivePointWise{
  %TypeHeaderCode
  #include "objectives/ObjectiveMSE.h"
  %End
  public:
    ObjectiveMSE();
    ~ObjectiveMSE();
    double getGradient (RecPred  * recPred);
};
