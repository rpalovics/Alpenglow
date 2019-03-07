#ifndef OBJECTIVE_LIST_RANK_H
#define OBJECTIVE_LIST_RANK_H

#include <vector>
#include "Objective.h"

using namespace std;

class ObjectiveListRank : public ObjectiveListWise{
  public:
    vector<double> get_gradient(vector<RecPred>* predictions);
  private:
    void clear();
    void compute_norms();
    void compute_gradients();
    double sigmoid(double x) {
      return 1.0 / (1.0 + exp(-x));
    };
    double sigmoid_der(double x) {
      return sigmoid(x) * ( 1 - sigmoid(x));
    };
    double prediction_norm, score_norm;
    vector<RecPred>* predictions;
    vector<double> gradients, predictions_exp, scores_exp;
};
#endif /* OBJECTIVE_LIST_RANK_H */
