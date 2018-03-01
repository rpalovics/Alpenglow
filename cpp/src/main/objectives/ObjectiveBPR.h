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
    pair <double,double> get_gradient(RecPred * rec_pred1, RecPred* rec_pred2);
};

#endif
