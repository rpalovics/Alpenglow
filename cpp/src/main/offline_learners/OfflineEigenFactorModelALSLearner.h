#ifndef OFFLINE_EIGEN_FACTOR_MODEL_ALS_LEARNER_H
#define OFFLINE_EIGEN_FACTOR_MODEL_ALS_LEARNER_H

#include "OfflineLearner.h"
#include "../models/factor/FactorModel.h"
#include "../models/factor/EigenFactorModel.h"
#include <gtest/gtest_prod.h>

using namespace std;

struct OfflineEigenFactorModelALSLearnerParameters{
  double number_of_iterations=3;
  double regularization_lambda=0.00001;
  double alpha=40;
  int implicit=1;
  int clear_before_fit=0;
};

class OfflineEigenFactorModelALSLearner : public OfflineLearner{
  public:
    OfflineEigenFactorModelALSLearner(OfflineEigenFactorModelALSLearnerParameters* parameters)
    : number_of_iterations_(parameters->number_of_iterations),
      regularization_lambda_(parameters->regularization_lambda),
      alpha_(parameters->alpha),
      implicit_(parameters->implicit),
      clear_before_fit_(parameters->clear_before_fit)
    {}
    ~OfflineEigenFactorModelALSLearner(){};
    virtual void iterate(){};
    void fit(RecommenderData* recommender_data);
    void set_model(EigenFactorModel* model){ model_ = model; }
    void set_copy_from_model(FactorModel* model){ copy_from_model_ = model; }
    void set_copy_to_model(FactorModel* model){ copy_to_model_ = model; }
    bool self_test();
  private:
    //parameters
    const double number_of_iterations_;
    const double regularization_lambda_;
    const double alpha_;
    const int implicit_;
    int clear_before_fit_;

    //ials methods
    SparseMatrix<double> recommender_data_to_spmat_(RecommenderData* recommender_data);
    MatrixXdRM optimize_factors_(const MatrixXdRM &factors1, const SparseMatrix<double> &A);
    MatrixXdRM optimize_factors_implicit_(const MatrixXdRM &factors1, const SparseMatrix<double> &A);
    MatrixXdRM optimize_factors_explicit_(const MatrixXdRM &factors1, const SparseMatrix<double> &A);

    //traditional factor model copy
    void do_copy_from_model_(FactorModel *model, MatrixXdRM &user_factors, MatrixXdRM &item_factors);
    void do_copy_to_model_(FactorModel *model, MatrixXdRM &user_factors, MatrixXdRM &item_factors);

    //components
    EigenFactorModel* model_ = NULL;
    FactorModel* copy_from_model_ = NULL;
    FactorModel* copy_to_model_ = NULL;

    //friends
    FRIEND_TEST(TestEigenFactorModel, testOfflineEigenFactorModelALSLearner);
    FRIEND_TEST(TestEigenFactorModel, testOfflineEigenFactorModel);
};

#endif
