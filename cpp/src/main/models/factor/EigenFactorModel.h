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
    void add(RecDat* rec_dat) override;
    double prediction(RecDat* rec_dat) override;
    void write(ofstream& file) override;
    void read(ifstream& file) override;
    void clear() override;
    bool autocalled_initialize() override { clear(); return true;}
    bool self_test(){return true;}
  protected:
    //parameters
    const int dimension_;
    const double begin_min_;
    const double begin_max_;
    const int seed_;

    //state
    EigenFactors user_factors_, item_factors_;
    Eigen::SparseMatrix<double> user_item_matrix_rows_;
    Eigen::SparseMatrix<double,Eigen::RowMajor> user_item_matrix_cols_;

    //friends
    friend class EigenFactorModelUpdater;
};

#endif
