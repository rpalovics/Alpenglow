struct OfflineEigenFactorModelALSLearnerParameters{
  %TypeHeaderCode
  #include "offline_learners/OfflineEigenFactorModelALSLearner.h"
  %End
  double number_of_iterations;
  double regularization_lambda;
  double alpha;
  int implicit;
  int clear_before_fit;
};

class OfflineEigenFactorModelALSLearner : public OfflineLearner{
  %TypeHeaderCode
  #include "offline_learners/OfflineEigenFactorModelALSLearner.h"
  %End
  public:
    OfflineEigenFactorModelALSLearner(OfflineEigenFactorModelALSLearnerParameters* parameters);
    ~OfflineEigenFactorModelALSLearner();
    virtual void iterate();
    void fit(RecommenderData* recommender_data);
    void set_model(EigenFactorModel* model);
    bool self_test();
};
