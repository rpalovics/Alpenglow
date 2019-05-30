#ifndef SVDPP_MODEL_H
#define SVDPP_MODEL_H

#include <unordered_map>
#include <stdexcept>
#include "../../utils/Factors.h"
#include "../../utils/Util.h"
#include "../../utils/UserHistory.h"
#include "../../general_interfaces/NeedsExperimentEnvironment.h"
#include "../../general_interfaces/Initializable.h"
#include "../Model.h"
#include <gtest/gtest_prod.h>

using namespace std;


//SIP_AUTOCONVERT
/// SVD++ algorithm

struct SvdppModelParameters {
  int dimension = 10;
  double begin_min = -0.1;
  double begin_max = 0.1;
  bool use_sigmoid = false;
  double user_vector_weight = 1;
  double history_weight = 1;
  string norm_type = "constant";
  double gamma = 0.8;
  int seed=745578;
  int initialize_all = false;
  int max_item = -1;
  int max_user = -1;
};

class SvdppModel
: public Model
, public NeedsExperimentEnvironment
, public Initializable
{
  public:
    SvdppModel(SvdppModelParameters *parameters):
      dimension_(parameters->dimension),
      begin_min_(parameters->begin_min),
      begin_max_(parameters->begin_max),
      initialize_all_(parameters->initialize_all),
      max_user_(parameters->max_user),
      max_item_(parameters->max_item),
      use_sigmoid_(parameters->use_sigmoid),
      user_vector_weight_(parameters->user_vector_weight),
      history_weight_(parameters->history_weight),
      norm_type_(parameters->norm_type),
      gamma_(parameters->gamma)
    {
      set_parameters(parameters);
      clear();
    };
    void add(RecDat* rec_dat) override;
    double prediction(RecDat* rec_dat) override;
    void write(ostream& file) override;
    void read(istream& file) override;
    void clear() override;
    //double user_factor_mean();
    //double item_factor_mean();
    //void to_call_outside(RecDat*, int, std::vector<double>*);
    //double compute_product(RecDat * rec_dat);
    //double get_history_weight(int num);
    //double compute_user_bias(RecDat * rec_dat);
    //double compute_item_bias(RecDat * rec_dat);
    bool self_test(){
      bool ok=Model::self_test();
      if(initialize_all_ and (max_item_==-1 or max_user_ ==-1)){
        ok = false;
        cerr << "SvdppModel::initialize_all_ is not used properly." << endl;
      }
      if(begin_min_ == -1 or begin_max_ == -1){
        ok = false;
        cerr << "SvdppModel::begin_min_ and SvdppModel::begin_max_ is not set properly." << endl;
      }
      if(dimension_ <= 0){
        ok = false;
        cerr << "SvdppModel::dimension_ <= 0." << endl;
      }
      if(norm_type_!="disabled" and norm_type_!="constant" and norm_type_!="recency" and norm_type_!="exponential" and norm_type_!="youngest"){ ok=false; cerr << "AsymmetricFactorModel::norm_type is not properly set, it should be one of the following: disabled, constant, recency, exponential, youngest." << endl; }
      if(norm_type_=="exponential" and gamma_==-1){ ok=false; cerr << "SvdppModel::gamma is not set." << endl; }
      if(norm_type_=="exponential" and gamma_==1){ ok=false; cerr << "SvdppModel::gamma==1. Hint: consider norm_type=constant." << endl; }
      if(user_vector_weight_==-1 or history_weight_==-1){ cerr << "WARNING: SvdppModel::user_vector_weight or SvdppModel::history_weight is not set." << endl; }
      if(user_vector_weight_==0){ cerr << "SvdppModel::user_vector_weight_=0 makes model identical to AsymmetricFactorModel." << endl; }
      if(history_weight_==0){ cerr << "SvdppModel::history_weight_=0 makes model identical to FactorModel." << endl; }
      return ok;
    }
  protected:
    //parameters
    const int dimension_;
    const double begin_min_, begin_max_;
    const bool use_sigmoid_;
    int initialize_all_;
    int max_user_;
    int max_item_;
    const double user_vector_weight_;
    const double history_weight_;
    const string norm_type_;
    const double gamma_;
    FactorsParameters factors_parameters;

    //state
    Factors user_factors_, item_factors_, history_item_factors_;
    UserHistory user_history_container_;
    vector<double> cached_user_factor_;
    vector<double> cached_weights_;
    double cached_norm_;
    int last_user_, last_time_, last_id_;
    bool invalidate_user_factor_;
    //double cached_norm_, last_timestamp_;
    //vector<double> user_vector_cache_;
    //int last_user_, last_recdat_id_;
    //vector<vector<int>*> user_history_;

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
    void set_parameters(SvdppModelParameters* parameters);
    void compute_user_factor(RecDat* rec_dat);
    double compute_norm(int user_activity_size);
    bool cache_is_valid(RecDat* rec_dat);
    vector<double> compute_histvector_sum(RecDat* rec_dat, const vector<const RecDat*>* user_history);


    friend class SvdppModelGradientUpdater;
    friend class SvdppModelUpdater;
    //friend class SvdppModelFilter;
    //friend class TestSvdppModel;
    //FRIEND_TEST(TestSvdppModel, test);
    //FRIEND_TEST(TestSvdppModel, similarity);
    //FRIEND_TEST(TestSvdppModelFilter, test_basic);
    //FRIEND_TEST(TestSvdppModelFilter, test_all);
};

#endif /* SVDPP_MODEL_H */
