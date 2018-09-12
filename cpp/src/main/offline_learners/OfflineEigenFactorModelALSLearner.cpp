#include "OfflineEigenFactorModelALSLearner.h"
#include <eigen3/Eigen/Sparse>
#include <vector>

using namespace Eigen;

bool OfflineEigenFactorModelALSLearner::self_test(){
  bool OK = true;
  if(model_ == NULL){
    OK = false;
    cerr << "OfflineEigenFactorModelALSLearner::model_ not set"<<endl;
  }

  return OK;
}

SparseMatrix<double> OfflineEigenFactorModelALSLearner::recommender_data_to_spmat_(RecommenderData* recommender_data){
  vector<RecDat> *rec_data = recommender_data->get_rec_data();
  vector<Triplet<double>> coefs;
  coefs.reserve(rec_data->size());

  int maxuser = 0;
  int maxitem = 0;
  for(RecDat &r : *rec_data){
    maxuser = max(maxuser, r.user);
    maxitem = max(maxitem, r.item);
    coefs.push_back(Triplet<double>(r.user, r.item, r.score));
  }

  SparseMatrix<double> A(maxuser+1,maxitem+1);

  if(implicit_){
    A.setFromTriplets(coefs.begin(), coefs.end(), [](const double &a, const double &b){return a+b;});
  } else {
    A.setFromTriplets(coefs.begin(), coefs.end(), [](const double &a, const double &b){return b;});
  }
  return A;
}

MatrixXdRM OfflineEigenFactorModelALSLearner::optimize_factors_implicit_(const MatrixXdRM &factors1, const SparseMatrix<double> &A){
  auto transf = [this](double val){return val*alpha_+1;};
  int dim = factors1.cols();

  MatrixXd YTY = factors1.transpose()*factors1;
  MatrixXd kxkLambdadiag = VectorXd::Constant(dim,1,regularization_lambda_*A.nonZeros()).asDiagonal();
  MatrixXdRM factors2optRM(A.cols(), factors1.cols());
  for (int k=0; k<A.outerSize(); ++k){
    int nonzeros = 0;
    if(A.isCompressed()){
      nonzeros = A.outerIndexPtr()[k+1]-A.outerIndexPtr()[k];
    } else {
      nonzeros = A.innerNonZeroPtr()[k];
    }

    MatrixXd YTxCU = MatrixXd::Constant(dim,nonzeros,0);
    MatrixXdRM Yreduced = MatrixXd::Constant(nonzeros,dim,0);
    MatrixXd YTxCUxPU = MatrixXd::Constant(dim,1,0);

    int i=0;
    for (SparseMatrix<double>::InnerIterator it(A,k); it; ++it){
      MatrixXd YTrow = factors1.row(it.row());
      MatrixXd YTrowT = YTrow.transpose();
      YTxCU.col(i) = YTrowT*(transf(it.value())-1);
      Yreduced.row(i) = YTrow;
      YTxCUxPU += YTrowT*transf(it.value());
      i++;
    }
    MatrixXd toInvert = YTY + YTxCU * Yreduced + kxkLambdadiag;
    PartialPivLU<MatrixXd> lu(toInvert);
    MatrixXd xu = lu.solve(YTxCUxPU);
    factors2optRM.row(k) = xu.transpose();
  }

  return factors2optRM;
}

MatrixXdRM OfflineEigenFactorModelALSLearner::optimize_factors_explicit_(const MatrixXdRM &factors1, const SparseMatrix<double> &A){
  int dim = factors1.cols();
  MatrixXd kxkLambdadiag = VectorXd::Constant(dim,1,regularization_lambda_*A.nonZeros()).asDiagonal();
  MatrixXdRM factors2optRM(A.cols(), factors1.cols());
  for (int k=0; k<A.outerSize(); ++k){
    int nonzeros = 0;
    if(A.isCompressed()){
      nonzeros = A.outerIndexPtr()[k+1]-A.outerIndexPtr()[k];
    } else {
      nonzeros = A.innerNonZeroPtr()[k];
    }

    MatrixXd YTxPU = MatrixXd::Constant(dim,1,0);
    MatrixXd YTreduced = MatrixXd::Constant(dim,nonzeros,0);
    int i=0;
    for (SparseMatrix<double>::InnerIterator it(A,k); it; ++it){
      MatrixXd row = factors1.row(it.row()).transpose();
      YTxPU += row*it.value();
      YTreduced.col(i) = factors1.row(it.row());
      i++;
    }
    MatrixXd xu = (YTreduced * YTreduced.transpose() + kxkLambdadiag).llt().solve(YTxPU);
    factors2optRM.row(k) = xu.transpose();
  }
  return factors2optRM;
}

MatrixXdRM OfflineEigenFactorModelALSLearner::optimize_factors_(const MatrixXdRM &factors1, const SparseMatrix<double> &A){
  if(implicit_){
    return optimize_factors_implicit_(factors1, A);
  } else {
    return optimize_factors_explicit_(factors1, A);
  }
}

void OfflineEigenFactorModelALSLearner::fit(RecommenderData* recommender_data){
  if(clear_before_fit_){
    model_->clear();
  }

  SparseMatrix<double> A = recommender_data_to_spmat_(recommender_data);
  SparseMatrix<double> AT = A.transpose();

  vector<bool> seen_users(A.rows());
  vector<bool> seen_items(A.cols());
  vector<RecDat> *rec_data = recommender_data->get_rec_data();
  for(RecDat &r : *rec_data){
    seen_users[r.user] = true;
    seen_items[r.item] = true;
  }

  model_->resize(A.rows(), A.cols());

  MatrixXdRM user_factors = model_->get_user_factors().factors;
  MatrixXdRM item_factors = model_->get_item_factors().factors;

  if(copy_from_model_ != NULL){
    do_copy_from_model_(copy_from_model_, user_factors, item_factors);
  }
  
  for(int i=0; i<number_of_iterations_; i++){
    user_factors = optimize_factors_(item_factors, AT);
    item_factors = optimize_factors_(user_factors, A);
  }

  if(copy_to_model_ != NULL){
    do_copy_to_model_(copy_to_model_, user_factors, item_factors);
  }

  model_->set_user_factors(user_factors, seen_users);
  model_->set_item_factors(item_factors, seen_items);
}

void OfflineEigenFactorModelALSLearner::do_copy_from_model_(FactorModel *model, MatrixXdRM &user_factors, MatrixXdRM &item_factors){
  for(int i=0; i<model->user_factors_.get_size(); i++){
    vector<double> *row = model->user_factors_.get(i);
    if(row != NULL){
      double* ptr = row->data();
      user_factors.row(i) = Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(ptr,row->size());
    }
  }
  for(int i=0; i<model->item_factors_.get_size(); i++){
    vector<double> *row = model->item_factors_.get(i);
    if(row != NULL){
      double* ptr = row->data();
      item_factors.row(i) = Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(ptr,row->size());
    }
  }
}

void OfflineEigenFactorModelALSLearner::do_copy_to_model_(FactorModel *model, MatrixXdRM &user_factors, MatrixXdRM &item_factors){
  vector<double> tmp;
  MatrixXdRM row;
  for(uint i=0; i<user_factors.rows(); i++){
    row = user_factors.row(i);
    tmp = vector<double>(row.data(), row.data()+row.cols());
    if(model->user_factors_.get_size() <= (int)i){
      model->user_factors_.init(i);
    }
    model->user_factors_.set(i, &tmp);
  }
  for(uint i=0; i<item_factors.rows(); i++){
    row = item_factors.row(i);
    tmp = vector<double>(row.data(), row.data()+row.cols());
    if(model->item_factors_.get_size() <= (int)i){
      model->item_factors_.init(i);
    }
    model->item_factors_.set(i, &tmp);
    model->lemp_container_.schedule_update_item(i);
  }
}

