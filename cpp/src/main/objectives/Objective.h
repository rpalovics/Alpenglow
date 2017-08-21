#ifndef OBJECTIVE
#define OBJECTIVE


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <cmath>
#include <limits>
#include "../utils/SpMatrix.h"
#include "../recommender_data/RecommenderData.h"


class ObjectivePointWise{
  public:
    ObjectivePointWise(){};
    virtual double get_gradient(RecPred * rec_pred) = 0;
};


class ObjectivePairWise{
  public:
    ObjectivePairWise(){}; 
    virtual pair <double,double> get_gradient(RecPred * rec_pred1, RecPred* rec_pred2)=0;
};

class ObjectiveListWise{
  public:
    ObjectiveListWise(){};
    virtual vector <double>  get_gradient(vector <RecPred> * predictions)=0;
};
#endif
