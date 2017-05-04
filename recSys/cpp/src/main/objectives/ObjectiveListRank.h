#ifndef OBJECTIVE_LIST_RANK
#define OBJECTIVE_LIST_RANK

#include <vector>
#include "Objective.h"

using namespace std;

class ObjectiveListRank : public ObjectiveListWise{
  public:
    ObjectiveListRank(){};
    ~ObjectiveListRank(){};
    vector <double> getGradient(vector <RecPred> * _predictions);
  private:
    void clear();
    void computeNorms();
    void computeGradients();
    double sigmoid(double x) {
      return 1.0 / (1.0 + exp(-x));
    };
    double sigmoidDer(double x) {
      return sigmoid(x) * ( 1 - sigmoid(x));
    };
    double predictionNorm, scoreNorm;
    vector <RecPred> * predictions;
    vector <double> gradients, predictionsExp, scoresExp;
};
#endif
