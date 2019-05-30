#ifndef EIGEN_FACTOR_MODEL_H
#define EIGEN_FACTOR_MODEL_H

#include <unordered_map>
#include <eigen3/Eigen/SparseCore>
#include "../../utils/EigenFactors.h"
#include "../../utils/Util.h"
#include "../Model.h"
#include "../RankingScoreIterator.h"
#include "FactorModelRankingScoreIterator.h"
#include "../TopListRecommender.h"
#include "lemp/EigenFactorsLempContainer.h"

//SIP_AUTOCONVERT

using namespace std;

struct EigenFactorModelParameters {
  int dimension=10;
  double begin_min=-0.01;
  double begin_max=0.01;
  int seed=745578;
  int lemp_bucket_size=64;
};

class EigenFactorModel
: public Model,
  virtual public RankingScoreIteratorProvider,
  public ToplistFromRankingScoreRecommender,
  public Initializable
{
  public:
    EigenFactorModel(EigenFactorModelParameters *parameters):
      dimension_(parameters->dimension),
      begin_min_(parameters->begin_min),
      begin_max_(parameters->begin_max),
      seed_(parameters->seed),
      lemp_bucket_size_(parameters->lemp_bucket_size),
      lemp_container_(&item_factors_, parameters->lemp_bucket_size)
    {
      EigenFactorsParameters factors_parameters;
      factors_parameters.seed = seed_;
      user_factors_.set_parameters(&factors_parameters);
      factors_parameters.seed += 1;
      item_factors_.set_parameters(&factors_parameters);
    };
    void add(RecDat* rec_dat) override{};
    double prediction(RecDat* rec_dat) override;
    void write(ostream& file) override;
    void read(istream& file) override;
    void clear() override;
    bool self_test(){return true;}
    void resize(int users, int items);

    const EigenFactors& get_user_factors(){return user_factors_;}
    const EigenFactors& get_item_factors(){return item_factors_;}
    void set_user_factors(const MatrixXdRM& factors, vector<bool> seen);
    void set_item_factors(const MatrixXdRM& factors, vector<bool> seen);

    RankingScoreIterator* get_ranking_score_iterator(int u) override;
  protected:
    bool autocalled_initialize() override { clear(); return true;}
    //parameters
    const int dimension_;
    const double begin_min_;
    const double begin_max_;
    const int seed_;
    int lemp_bucket_size_;

    //state
    EigenFactors user_factors_, item_factors_;
    EigenFactorsLempContainer lemp_container_;
    FactorModelRankingScoreIterator ranking_score_iterator_;

    //friends
    FRIEND_TEST(TestEigenFactorModel, testOfflineEigenFactorModel);
};

#endif /* EIGEN_FACTOR_MODEL_H */
