#ifndef FACTOR_MODEL
#define FACTOR_MODEL

#include <unordered_map>
#include "../../utils/Factors.h"
#include "../../utils/Util.h"
#include "../../utils/Bias.h"
#include "../../utils/Recency.h"
#include "../Model.h"
#include "../SimilarityModel.h"
#include "../RankingScoreIterator.h"
#include "../TopListRecommender.h"
#include "../../ranking/lemp/FactorsLempContainer.h"
#include "FactorModelRankingScoreIterator.h"
#include <gtest/gtest_prod.h>

using namespace std;


struct FactorModelParameters{
  int dimension;
  double begin_min, begin_max;
  bool use_sigmoid;
  bool use_item_bias, use_user_bias;
  bool initialize_all;
  int max_item, max_user;
  int seed=67439852;
  int lemp_bucket_size = 64;
  FactorModelParameters(){ //setting all to jinjactor default value
    dimension=-1;begin_min=-1;begin_max=-1;
    use_sigmoid=false;use_item_bias=false;use_user_bias=false;
    initialize_all=false;max_item=-1;max_user=-1;
  }
};

class FactorModel 
  : public Model, 
    public SimilarityModel,
    public Initializable,
    virtual public RankingScoreIteratorProvider,
    public ToplistFromRankingScoreRecommender
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
    //~FactorModel() { delete user_factors; delete item_factors; }
    void add(RecDat* rec_dat) override;
    double prediction(RecDat* rec_dat) override;
    double similarity(int item1, int item2) override;
    void write(ostream& file) override;
    void read(istream& file) override;
    void clear() override;
    void set_user_recency(Recency *recency){user_recency_ = recency;}
    void set_item_recency(Recency *recency){item_recency_ = recency;}
    bool self_test(){ //Ezek egyike sem feltetlen hiba, esetleg kevesbe szigoruva kene tenni.
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
    
    RankingScoreIterator* get_ranking_score_iterator(int u) override;
  protected:
    //parameters
    const int dimension_;
    const double begin_min_;
    const double begin_max_;
    const bool use_sigmoid_;
    const bool initialize_all_;
    const int max_user_;
    const int max_item_;
    const bool use_item_bias_;
    const bool use_user_bias_;

    //state
    Factors user_factors_, item_factors_;
    Bias user_bias_, item_bias_;
    Recency *user_recency_, *item_recency_;
    FactorsLempContainer lemp_container_;
    FactorModelRankingScoreIterator ranking_score_iterator_;

    //other
    //double user_factor_mean();
    //double item_factor_mean();
    bool autocalled_initialize() override {
      clear();
      return true;
    }
    void set_parameters(FactorModelParameters * parameters);
    double compute_product(RecDat * rec_dat);
    double compute_user_bias(RecDat * rec_dat);
    double compute_item_bias(RecDat * rec_dat);

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

#endif
