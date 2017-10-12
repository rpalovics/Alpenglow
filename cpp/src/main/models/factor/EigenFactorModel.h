#ifndef EIGEN_FACTOR_MODEL
#define EIGEN_FACTOR_MODEL

#include <unordered_map>
#include "../../utils/EigenFactors.h"
#include "../../utils/Util.h"
#include "../Model.h"
#include <eigen3/Eigen/SparseCore>

using namespace std;

struct EigenFactorModelParameters{
  int dimension=10;
  double begin_min=-0.01;
  double begin_max=0.01;
  int seed=67439852;
};

class EigenFactorModel : public Model, public Initializable {
  public:
    EigenFactorModel(EigenFactorModelParameters *parameters):
      dimension_(parameters->dimension),
      begin_min_(parameters->begin_min),
      begin_max_(parameters->begin_max),
      seed_(parameters->seed)
    {};
    void add(RecDat* rec_dat) override{};
    double prediction(RecDat* rec_dat) override;
    void write(ofstream& file) override;
    void read(ifstream& file) override;
    void clear() override;
    bool autocalled_initialize() override { clear(); return true;}
    bool self_test(){return true;}
    void resize(int users, int items);

    const EigenFactors& get_user_factors(){return user_factors_;}
    const EigenFactors& get_item_factors(){return item_factors_;}
    void set_user_factors(const EigenFactors& factors){user_factors_ = factors;}
    void set_item_factors(const EigenFactors& factors){item_factors_ = factors;}
    void set_user_factors(const MatrixXdRM& factors){user_factors_.factors = factors;}
    void set_item_factors(const MatrixXdRM& factors){item_factors_.factors = factors;}
  protected:
    //parameters
    const int dimension_;
    const double begin_min_;
    const double begin_max_;
    const int seed_;

    //state
    EigenFactors user_factors_, item_factors_;

    //friends
    FRIEND_TEST(TestEigenFactorModel, testOfflineEigenFactorModel);
};

#endif
