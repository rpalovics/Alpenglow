#ifndef FACTOR_MODEL_H
#define FACTOR_MODEL_H

#include <unordered_map>
#include "../../utils/Factors.h"
#include "../../utils/Util.h"
#include "../../utils/Bias.h"
#include "../../utils/Recency.h"
#include "../../general_interfaces/NeedsExperimentEnvironment.h"
#include "../../general_interfaces/Initializable.h"
#include "../Model.h"
#include "../SimilarityModel.h"
#include "../RankingScoreIterator.h"
#include "../TopListRecommender.h"
#include "lemp/FactorsLempContainer.h"
#include "FactorModelRankingScoreIterator.h"
#include <gtest/gtest_prod.h>

//SIP_AUTOCONVERT

using namespace std;

struct FactorModelParameters {
  int dimension = 10;
  double begin_min = -0.1;
  double begin_max = 0.1;
  bool use_sigmoid = false;
  bool use_item_bias = false;
  bool use_user_bias = false;
  int seed=745578;
  int lemp_bucket_size = 64;
  int initialize_all = -1;
  int max_item = -1;
  int max_user = -1;
};

class FactorModel
  : public Model, 
    public SimilarityModel,
    public NeedsExperimentEnvironment,
    virtual public RankingScoreIteratorProvider,
    public ToplistFromRankingScoreRecommender,
    public Initializable
{
  public:
    FactorModel(FactorModelParameters *parameters):
      dimension_(parameters->dimension),
      begin_min_(parameters->begin_min),
      begin_max_(parameters->begin_max),
      use_sigmoid_(parameters->use_sigmoid),
      initialize_all_(parameters->initialize_all),
      max_user_(parameters->max_user),
      max_item_(parameters->max_item),
      use_item_bias_(parameters->use_item_bias),
      use_user_bias_(parameters->use_user_bias),
      lemp_container_(&item_factors_, parameters->lemp_bucket_size)
    {
      set_parameters(parameters);
    };
    void add(RecDat* rec_dat) override;
    double prediction(RecDat* rec_dat) override;
    double similarity(int item1, int item2) override;
    void write(ostream& file) override;
    void read(istream& file) override;
    void clear() override;
    void set_user_recency(Recency *recency){user_recency_ = recency;}
    void set_item_recency(Recency *recency){item_recency_ = recency;}
    RankingScoreIterator* get_ranking_score_iterator(int u) override;
    bool self_test(){ //Not all are not fatal errors, should be less strict
      bool ok = Model::self_test() and SimilarityModel::self_test();
      if(initialize_all_ and (max_user_==-1 or max_item_==-1)){
        ok = false;
        cerr << "FactorModel::initialize_all_ is not used properly." << endl;
      }
      if(begin_min_ == -1 and begin_max_ == -1){
        ok = false;
        cerr << "FactorModel::begin_min_ and FactorModel::begin_max_ is not set properly." << endl;
      }
      if(dimension_ <= 0){
        ok = false;
        cerr << "FactorModel::dimension_ <= 0." << endl;
      }
      return ok;
    }
    
  protected:
    //parameters
    const int dimension_;
    const double begin_min_;
    const double begin_max_;
    const bool use_sigmoid_;
    int initialize_all_;
    int max_user_;
    int max_item_;
    const bool use_item_bias_;
    const bool use_user_bias_;

    //state
    Factors user_factors_, item_factors_;
    Bias user_bias_, item_bias_;
    Recency *user_recency_ = NULL, *item_recency_ = NULL;
    FactorsLempContainer lemp_container_;
    FactorModelRankingScoreIterator ranking_score_iterator_;

    //other
    bool autocalled_initialize() override {
      if (-1==initialize_all_){
        if (NULL==experiment_environment_) return false;
        initialize_all_ = experiment_environment_->get_initialize_all();
      }
      if(initialize_all_ && max_item_==-1){
        if (NULL==experiment_environment_) return false;
        max_item_ = experiment_environment_->get_max_item_id();
      }
      if(initialize_all_ && max_user_==-1){
        if (NULL==experiment_environment_) return false;
        max_user_ = experiment_environment_->get_max_user_id();
      }
      clear();
      return true;
    }
    void set_parameters(FactorModelParameters* parameters);
    double compute_product(RecDat* rec_dat);
    double compute_user_bias(RecDat* rec_dat);
    double compute_item_bias(RecDat* rec_dat);

    //friends
    friend class FactorModelGradientUpdater;
    friend class PosNegFactorModelGradientUpdater;
    friend class FactorModelUpdater;
    friend class FactorModelFilter;
    friend class FactorToAMFAdapter;
    friend class OfflineEigenFactorModelALSLearner;
    FRIEND_TEST(TestFactorModel, test);
    FRIEND_TEST(TestFactorModel, similarity);
    FRIEND_TEST(TestFactorModelFilter, test_basic);
    FRIEND_TEST(TestFactorModelFilter, test_recency);
    FRIEND_TEST(TestFactorModelFilter, test_bias);
    FRIEND_TEST(TestFactorModelFilter, test_sigmoid);
    FRIEND_TEST(TestFactorModelFilter, test_all);
};

#endif /* FACTOR_MODEL_H */
