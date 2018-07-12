#ifndef OBJECTIVE
#define OBJECTIVE

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderData.h"


class ObjectivePointWise{
  public:
    virtual ~ObjectivePointWise(){}
    virtual double get_gradient(RecPred* rec_pred)=0;
};


class ObjectivePairWise{ //SIP_ABSTRACT
  public:
    virtual ~ObjectivePairWise(){}
    virtual pair<double,double> get_gradient(RecPred* rec_pred1, RecPred* rec_pred2)=0;
};

class ObjectiveListWise{
  public:
    virtual ~ObjectiveListWise(){}
    virtual vector<double>  get_gradient(vector<RecPred>* predictions)=0;
};
#endif
