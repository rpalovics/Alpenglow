class ObjectivePointWise{
  %TypeHeaderCode
  #include "objectives/Objective.h"
  %End
  public:
    ObjectivePointWise();
    virtual double get_gradient(RecPred * rec_pred) = 0;
};

class ObjectivePairWise /Abstract/{
  %TypeHeaderCode
  #include "objectives/Objective.h"
  %End
  public:
    ObjectivePairWise(); 
    // virtual std::pair<double,double> get_gradient(RecPred * rec_pred1, RecPred* rec_pred2)=0;
};

class ObjectiveListWise{
  %TypeHeaderCode
  #include "objectives/Objective.h"
  %End
  public:
    ObjectiveListWise();
    virtual std::vector <double>  get_gradient(std::vector <RecPred> * predictions)=0;
};
