#ifndef OFFLINE_EIGEN_FACTOR_MODEL_ALS_LEARNER_H
#define OFFLINE_EIGEN_FACTOR_MODEL_ALS_LEARNER_H

#include "OfflineLearner.h"
#include "../models/factor/EigenFactorModel.h"

using namespace std;

struct OfflineEigenFactorModelALSLearnerParameters{
  double number_of_iterations=6;
  double regularization_gamma=500;
  double alpha=40;
  int implicit=1;
};

class OfflineEigenFactorModelALSLearner : public OfflineLearner{
  public:
    OfflineEigenFactorModelALSLearner(OfflineEigenFactorModelALSLearnerParameters* parameters)
    : number_of_iterations_(parameters->number_of_iterations),
      regularization_gamma_(parameters->regularization_gamma),
      alpha_(parameters->alpha),
      implicit_(parameters->alpha)
    {}
    ~OfflineEigenFactorModelALSLearner(){};
    virtual void iterate(){};
    void fit(RecommenderData* recommender_data){};
    void set_model(EigenFactorModel* model){ model_ = model; }
    bool self_test(){return true;}
  private:
    //parameters
    const double number_of_iterations_;
    const double regularization_gamma_;
    const double alpha_;
    const int implicit_;

    //components
    EigenFactorModel* model_ = NULL;
};

#endif
