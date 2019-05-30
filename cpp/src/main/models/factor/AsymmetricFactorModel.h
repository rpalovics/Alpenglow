#ifndef ASYMMETRIC_FACTOR_MODEL_H
#define ASYMMETRIC_FACTOR_MODEL_H

#include "../../utils/Factors.h"
#include "../../utils/Util.h"
#include "../../utils/SpMatrix.h"
#include "../../utils/UserHistory.h"
#include "../../general_interfaces/NeedsExperimentEnvironment.h"
#include "../../general_interfaces/Initializable.h"
#include "../Model.h"
#include <stdexcept>
#include <gtest/gtest_prod.h>

using namespace std;

//SIP_AUTOCONVERT

struct AsymmetricFactorModelParameters {
  int dimension = 10;
  double begin_min = -0.1;
  double begin_max = 0.1;
  bool use_sigmoid = false;
  string norm_type = "constant"; //disabled, constant, recency, exponential, youngest
  double gamma = 0.8;
  int seed = 745578;
  int initialize_all = -1;
  int max_item = -1;
};

class AsymmetricFactorModel
: public Model
, public NeedsExperimentEnvironment
, public Initializable
{
  public:
    AsymmetricFactorModel(AsymmetricFactorModelParameters* parameters)
      : norm_type_(parameters->norm_type)
      , gamma_(parameters->gamma)
      , use_sigmoid_(parameters->use_sigmoid)
      , dimension_(parameters->dimension)
      , initialize_all_(parameters->initialize_all)
      , max_item_(parameters->max_item)
    {
      set_parameters(parameters);
    }
    void add(RecDat* rec_dat) override;
    double prediction(RecDat* rec_dat) override;
    void write(ostream& file) override;
    void read(istream& file) override;
    void clear() override;
    bool self_test(){ //TODO test random
      bool ok = Model::self_test();
      if(initialize_all_ and max_item_<0){
        ok = false;
        cerr << "AsymmetricFactorModel::initialize_all_ is not used properly." << endl;
      }
      if(factors_parameters_.begin_min == factors_parameters_.begin_max){
        ok = false;
        cerr << "AsymmetricFactorModel::begin_min_ and FactorModel::begin_max_ is not set properly." << endl;
      }
      if(dimension_ <= 0){
        ok = false;
        cerr << "AsymmetricFactorModel::dimension_ <= 0." << endl;
      }
      if(norm_type_!="disabled" and norm_type_!="constant" and norm_type_!="recency" and norm_type_!="exponential" and norm_type_!="youngest"){ ok=false; cerr << "AsymmetricFactorModel::norm_type is not properly set, it should be one of the following: disabled, constant, recency, exponential, youngest." << endl; }
      return ok;
    }
  protected:
    bool autocalled_initialize() override {
      if (-1==initialize_all_){
        if (NULL==experiment_environment_) return false;
        initialize_all_ = experiment_environment_->get_initialize_all();
      }
      if(initialize_all_ && max_item_==-1){
        if (NULL==experiment_environment_) return false;
        max_item_ = experiment_environment_->get_max_item_id();
      }
      clear();
      return true;
    }
    void set_parameters(AsymmetricFactorModelParameters* parameters);
    void compute_user_factor(RecDat* rec_dat);
    double compute_norm(int user_activity_size);
    bool cache_is_valid(RecDat* rec_dat);
    vector<double> compute_histvector_sum(RecDat* rec_dat, const vector<const RecDat*>* user_history);

    //parameters
    const int dimension_;
    const bool use_sigmoid_;
    int initialize_all_;
    int max_item_;
    const string norm_type_;
    const double gamma_;
    FactorsParameters factors_parameters_, history_item_factors_parameters_;

    //state
    Factors history_item_factors_, item_factors_;
    vector<double> cached_user_factor_;
    vector<double> cached_weights_;
    double cached_norm_;
    int last_user_, last_time_, last_id_;
    bool invalidate_user_factor_;
    UserHistory user_history_container_;

    friend class AsymmetricFactorModelGradientUpdater;
    friend class AsymmetricFactorModelUpdater;
    FRIEND_TEST(TestAsymmetricFactorModel, test_weights);
};

#endif /* ASYMMETRIC_FACTOR_MODEL_H */
