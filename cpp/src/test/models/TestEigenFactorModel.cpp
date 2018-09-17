#include <gtest/gtest.h>
#include <stdio.h>
#include <fstream>
#include <random>
#include <utility>
#include "../../main/models/factor/EigenFactorModel.h"
#include "../../main/models/factor/FactorModel.h"
#include "../../main/offline_learners/OfflineEigenFactorModelALSLearner.h"
#include "../../main/utils/EigenFactors.h"

namespace {
class TestEigenFactorModel : public ::testing::Test { 
  public:
    vector<RecDat*> recDats;

    TestEigenFactorModel(){}
    virtual ~TestEigenFactorModel(){}
    virtual void SetUp(){}
    RecDat* createRecDat(int user, int item, double time, double score){
      RecDat* recDat = new RecDat;
      recDat -> user = user;
      recDat -> item = item;
      recDat -> time = time;
      recDat -> category = 0;
      recDat -> score = score;
      recDats.push_back(recDat);
      return recDat;
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = recDats.begin();it!=recDats.end();it++){
        delete *it;
      }
    }

};

}

TEST_F(TestEigenFactorModel, testEigenFactors){
  EXPECT_DOUBLE_EQ(1,1);
  EigenFactorsParameters ep;
  ep.seed = 1345;
  EigenFactors ef(&ep);
  ef.reinitialize_factors(10, 50, -0.01, 0.01);
  for(int i=0; i<10; i++){
    for(int j=0; j<50; j++){
      EXPECT_GE(0.01, ef.factors(j,i));
      EXPECT_LE(-0.01, ef.factors(j,i));
    }
  }
  ef.reinitialize_factors(10, 50, -0.05, 0.01);
  for(int i=0; i<10; i++){
    for(int j=0; j<50; j++){
      EXPECT_GE(0.01, ef.factors(j,i));
      EXPECT_LE(-0.05, ef.factors(j,i));
    }
  }

  double randpos0 = ef.factors(0,0);
  double randpos1 = ef.factors(10,5);
  double randpos2 = ef.factors(10,6);
  double randpos3 = ef.factors(14,7);

  ef.resize(10,50,-0.05, 0.01);
  EXPECT_EQ(randpos0, ef.factors(0,0));
  EXPECT_EQ(randpos1, ef.factors(10,5));
  EXPECT_EQ(randpos2, ef.factors(10,6));
  EXPECT_EQ(randpos3, ef.factors(14,7));

  //write 2 factors
  double v1 = ef.factors(1,1);
  ofstream file("tempfile1");
  ef.write(file);

  ef.factors(1,1)=10;
  ef.write(file);
  file.close();

  //read 2 factors
  EigenFactors ef2(&ep);
  EigenFactors ef3(&ep);
  ifstream file2("tempfile1");
  ef2.read(file2);
  ef3.read(file2);
  file2.close();

  EXPECT_EQ(ef.factors.rows(), ef2.factors.rows());
  EXPECT_EQ(ef.factors.rows(), ef3.factors.rows());
  EXPECT_EQ(ef.factors.cols(), ef2.factors.cols());
  EXPECT_EQ(ef.factors.cols(), ef3.factors.cols());
  for(int i=0; i<ef.factors.rows(); i++){
    for(int j=0; j<ef.factors.cols(); j++){
      if(!(i==1) && (j==1)){
        EXPECT_EQ(ef.factors(i,j), ef2.factors(i,j));
        EXPECT_EQ(ef.factors(i,j), ef3.factors(i,j));
      }
    }
  }
  EXPECT_EQ(v1,ef2.factors(1,1));
  EXPECT_EQ(10,ef3.factors(1,1));
  remove("tempfile1");
}

#define USERS 100
#define ITEMS 170
#define DIM 10
#define LAMBDA 0.000001
#define ITEMPERU 8
#define ALPHA 40

double implicit_err(const MatrixXdRM &userf, const MatrixXdRM &itemf, SparseMatrix<double> &A){
  double errcorr = 0;
  double weight = 0;
  MatrixXd predm = userf * itemf.transpose() - A;
  for (int k=0; k<A.outerSize(); ++k)
    for (SparseMatrix<double>::InnerIterator it(A,k); it; ++it)
    {
      errcorr += pow(userf.row(it.row())*itemf.row(it.col()).transpose()-1,2)*it.value()*ALPHA;
      weight += it.value()*ALPHA;
    }
  double mainerr = pow(predm.squaredNorm() + errcorr, 1.0/2);
  return mainerr + LAMBDA*A.nonZeros()*userf.norm() + LAMBDA*A.nonZeros()*itemf.norm();
}

double explicit_err(const MatrixXd &userf, const MatrixXd &itemf, SparseMatrix<double> &A){
  double err = 0;
  for (int k=0; k<A.outerSize(); ++k)
    for (SparseMatrix<double>::InnerIterator it(A,k); it; ++it)
    {
      err += pow(userf.row(it.row())*itemf.row(it.col()).transpose()-it.value(),2);
    }
  return pow(err / A.nonZeros(),1.0/2) + LAMBDA*A.nonZeros()*userf.norm() + LAMBDA*A.nonZeros()*itemf.norm();
}

TEST_F(TestEigenFactorModel, testOfflineEigenFactorModelALSLearner){
  //to spmat
  RecommenderData data;
  vector<RecDat> rec_data;
  rec_data.push_back(*createRecDat(10,10,0,1));
  rec_data.push_back(*createRecDat(11,10,0,1));
  rec_data.push_back(*createRecDat(5,8,0,1));
  rec_data.push_back(*createRecDat(10,10,0,2));
  rec_data.push_back(*createRecDat(10,17,0,1));
  data.set_rec_data(rec_data);

  OfflineEigenFactorModelALSLearnerParameters parameters;
  OfflineEigenFactorModelALSLearner learner(&parameters);
  Eigen::SparseMatrix<double> A = learner.recommender_data_to_spmat_(&data);
  EXPECT_EQ(12,A.rows());
  EXPECT_EQ(18,A.cols());
  EXPECT_EQ(1,A.coeffRef(5,8));
  EXPECT_EQ(3,A.coeffRef(10,10));
  EXPECT_EQ(0,A.coeffRef(0,1));

  parameters.implicit = 0;
  OfflineEigenFactorModelALSLearner learner2(&parameters);
  Eigen::SparseMatrix<double> A2 = learner2.recommender_data_to_spmat_(&data);
  EXPECT_EQ(2,A2.coeffRef(10,10));

  //implicit optimize

  MatrixXd userf;
  MatrixXd itemf;
  userf = MatrixXdRM::Random(USERS, DIM);
  itemf = MatrixXdRM::Random(ITEMS, DIM);

  vector<Triplet<double>> coefs;
  int num = USERS*ITEMPERU;
  coefs.reserve(num);

  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> randuser(0,USERS-1);
  std::uniform_int_distribution<int> randitem(0,ITEMS-1);
  std::uniform_int_distribution<int> randscore(1,5);

  for(int it=0; it<num; it++){
    int u = randuser(rng);
    int i = randitem(rng);
    coefs.push_back(Triplet<double>(u, i, 1));
  }

  A = SparseMatrix<double>(USERS,ITEMS);
  A.setFromTriplets(coefs.begin(), coefs.end());
  SparseMatrix<double> AT = A.transpose();

  double err = implicit_err(userf,itemf,A);
  for(int i=0; i<5; i++){
    userf = learner.optimize_factors_implicit_(itemf, AT);
    itemf = learner.optimize_factors_implicit_(userf, A);

    double new_err = implicit_err(userf,itemf,A);
    EXPECT_LE(new_err, err);
    err = new_err;
  }

  //explicit optimize

  userf = MatrixXdRM::Random(USERS, DIM);
  itemf = MatrixXdRM::Random(ITEMS, DIM);

  coefs = vector<Triplet<double>>();
  coefs.reserve(num);
  for(int it=0; it<num; it++){
    int u = randuser(rng);
    int i = randitem(rng);
    int s = randscore(rng);
    coefs.push_back(Triplet<double>(u, i, s));
  }

  A = SparseMatrix<double>(USERS,ITEMS);
  A.setFromTriplets(coefs.begin(), coefs.end());
  AT = A.transpose();

  err = explicit_err(userf,itemf,A);
  for(int i=0; i<5; i++){
    userf = learner.optimize_factors_explicit_(itemf, AT);
    itemf = learner.optimize_factors_explicit_(userf, A);

    double new_err = explicit_err(userf,itemf,A);
    EXPECT_LE(new_err, err);
    err = new_err;
  }
}


TEST_F(TestEigenFactorModel, testOfflineEigenFactorModel){
  RecommenderData data;
  vector<RecDat> rec_data;
  rec_data.push_back(*createRecDat(10,10,0,1));
  rec_data.push_back(*createRecDat(11,10,0,1));
  rec_data.push_back(*createRecDat(5,8,0,1));
  rec_data.push_back(*createRecDat(10,10,0,2));
  rec_data.push_back(*createRecDat(10,17,0,1));
  data.set_rec_data(rec_data);

  OfflineEigenFactorModelALSLearnerParameters parameters;
  OfflineEigenFactorModelALSLearner learner(&parameters);

  EigenFactorModelParameters mparameters;
  EigenFactorModel model(&mparameters);
  FactorModelParameters lparameters;
  lparameters.dimension = 10;
  lparameters.begin_min = -1;
  lparameters.begin_max = 1;
  FactorModel lmodel(&lparameters);

  learner.set_model(&model);
  learner.set_copy_to_model(&lmodel);
  learner.fit(&data);
  auto u_factors = model.get_user_factors();
  auto i_factors = model.get_item_factors();
  EXPECT_EQ(12,u_factors.factors.rows());
  EXPECT_EQ(10,u_factors.factors.cols());
  EXPECT_EQ(18,i_factors.factors.rows());
  EXPECT_EQ(10,i_factors.factors.cols());

  //copy to successful
  RecDat *req = createRecDat(5,5,1,1);
  EXPECT_EQ(lmodel.prediction(req),model.prediction(req));

  //copy from successful
  OfflineEigenFactorModelALSLearnerParameters parameters_c;
  parameters_c.number_of_iterations=0;
  parameters_c.clear_before_fit=0;
  OfflineEigenFactorModelALSLearner learner2(&parameters_c);

  EigenFactorModelParameters mparameters_c;
  EigenFactorModel model_c(&mparameters_c);
  learner2.set_copy_from_model(&lmodel);
  learner2.set_model(&model_c);
  learner.fit(&data);
  EXPECT_EQ(lmodel.prediction(req),model.prediction(req));


  ofstream file("tempfile1");
  model.write(file);
  file.close();

  EigenFactorModelParameters parameters2;
  parameters2.seed=0;
  parameters2.begin_min=0;
  parameters2.begin_max=1;
  parameters2.dimension=0;
  EigenFactorModel model2(&parameters2);

  ifstream file2("tempfile1");
  model2.read(file2);
  file2.close();
  remove("tempfile1");

  EXPECT_EQ(model.dimension_, model2.dimension_);
  EXPECT_EQ(model.begin_min_, model2.begin_min_);
  EXPECT_EQ(model.begin_max_, model2.begin_max_);
  EXPECT_EQ(model.seed_, model2.seed_);
  RecDat rd;
  for(int i=0; i<12; i++){
    for(int j=0; j<18; j++){
      rd.user=i;
      rd.item=j;
      EXPECT_DOUBLE_EQ(model.prediction(&rd), model2.prediction(&rd));
    }
  }
}

TEST_F(TestEigenFactorModel, TestEigenFactorModelRanking){
  EigenFactorModelParameters mparameters;
  mparameters.dimension = 1;
  mparameters.lemp_bucket_size = 2;
  EigenFactorModel model(&mparameters);
  MatrixXdRM user_factor(2,1);
  user_factor << 1, -1;
  MatrixXdRM item_factor(6,1);
  item_factor << 1,6,2,5,3,4;
  model.set_user_factors(user_factor, vector<bool>(2, true));
  model.set_item_factors(item_factor, vector<bool>(6, true));

  vector<double> scores;
  vector<double> items;
  RankingScoreIterator* it = model.get_ranking_score_iterator(1);
  while(it->has_next()){
    int i;
    double s;
    tie(i, s) = it->get_next();
    scores.push_back(s);
    items.push_back(i);
  }
  vector<double> ideal_scores({-6,-5,-4,-3,-2,-1});
  vector<double> ideal_items({1,3,5,4,2,0});
  EXPECT_EQ(ideal_scores, scores);
  EXPECT_EQ(ideal_items, items);

  scores.clear();
  items.clear();
  it = model.get_ranking_score_iterator(0);
  while(it->has_next(2.1)){
    int i;
    double s;
    tie(i, s) = it->get_next();
    scores.push_back(s);
    items.push_back(i);
  }
  ideal_scores={6,5,4,3};
  ideal_items={1,3,5,4};
  EXPECT_EQ(ideal_scores, scores);
  EXPECT_EQ(ideal_items, items);
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
