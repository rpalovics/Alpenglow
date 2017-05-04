#ifndef OBJECTIVE_BPR
#define OBJECTIVE_BPR
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

#include "Objective.h"

using namespace std;

class ObjectiveBPR : public ObjectivePairWise{
  public:
    ObjectiveBPR(){};  
    ~ObjectiveBPR(){};  
    pair <double,double> getGradient(RecPred * recPred1, RecPred* recPred2);
  protected:
};

#endif
