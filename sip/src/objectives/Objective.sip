
class ObjectivePointWise{
  %TypeHeaderCode
  #include "objectives/Objective.h"
  %End
  public:
    virtual ~ObjectivePointWise();
    virtual double get_gradient(RecPred* rec_pred)=0;
    bool self_test();
};

class ObjectivePairWise /Abstract/{
  %TypeHeaderCode
  #include "objectives/Objective.h"
  %End
  public:
    virtual ~ObjectivePairWise();
//    virtual pair<double,double> get_gradient(RecPred* rec_pred1, RecPred* rec_pred2)=0;
    bool self_test();
};

class ObjectiveListWise{
  %TypeHeaderCode
  #include "objectives/Objective.h"
  %End
  public:
    virtual ~ObjectiveListWise();
    virtual std::vector<double>  get_gradient(std::vector<RecPred>* predictions)=0;
    bool self_test();
};

