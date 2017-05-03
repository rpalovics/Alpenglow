class ObjectivePointWise{
  %TypeHeaderCode
  #include "objectives/Objective.h"
  %End
  public:
    ObjectivePointWise();
    virtual double getGradient(RecPred * recPred) = 0;
};

class ObjectivePairWise /Abstract/{
  %TypeHeaderCode
  #include "objectives/Objective.h"
  %End
  public:
    ObjectivePairWise(); 
    // virtual std::pair<double,double> getGradient(RecPred * recPred1, RecPred* recPred2)=0;
};

class ObjectiveListWise{
  %TypeHeaderCode
  #include "objectives/Objective.h"
  %End
  public:
    ObjectiveListWise();
    virtual std::vector <double>  getGradient(std::vector <RecPred> * predictions)=0;
};
