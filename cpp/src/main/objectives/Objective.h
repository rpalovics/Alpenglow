#ifndef OBJECTIVE_POINT_WISE_H
#define OBJECTIVE_POINT_WISE_H

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"


class ObjectivePointWise{
  public:
    virtual ~ObjectivePointWise(){}
    virtual double get_gradient(RecPred* rec_pred)=0;
    bool self_test(){ return true; }
};


class ObjectivePairWise{ //SIP_ABSTRACT
  public:
    virtual ~ObjectivePairWise(){}
    virtual pair<double,double> get_gradient(RecPred* rec_pred1, RecPred* rec_pred2)=0;
    bool self_test(){ return true; }
};

class ObjectiveListWise{
  public:
    virtual ~ObjectiveListWise(){}
    virtual vector<double>  get_gradient(vector<RecPred>* predictions)=0;
    bool self_test(){ return true; }
};
#endif /* OBJECTIVE_POINT_WISE_H */
