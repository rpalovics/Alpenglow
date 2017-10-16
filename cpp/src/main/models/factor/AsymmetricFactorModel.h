#ifndef A_FACTOR_MODEL
#define A_FACTOR_MODEL

#include "../../utils/Factors.h"
#include "../../utils/Util.h"
#include "../../utils/SpMatrix.h"
#include "../../utils/UserHistory.h"
#include "../Model.h"
#include <gtest/gtest_prod.h>

using namespace std;


struct AsymmetricFactorModelParameters{
  int dimension;
  double begin_min, begin_max;
  bool initialize_all, use_sigmoid;
  int max_item;
  string norm_type; //disabled, constant, recency, exponential, youngest
  double gamma;
  int seed=928357823;
  AsymmetricFactorModelParameters(){
    dimension=-1;
    begin_min=-1; begin_max=-1;
    initialize_all=false; use_sigmoid=false;
    max_item=-1;
    norm_type="";
    gamma=-1;
  }
};

class AsymmetricFactorModel : public Model{
  public:
    AsymmetricFactorModel(AsymmetricFactorModelParameters *parameters)
      :norm_type_(parameters->norm_type)
      ,gamma_(parameters->gamma)
      ,use_sigmoid_(parameters->use_sigmoid)
      ,dimension_(parameters->dimension)
      ,initialize_all_(parameters->initialize_all)
      ,max_item_(parameters->max_item)
    {
      set_parameters(parameters);
      clear();
    };
    void add(RecDat* rec_dat) override;
    double prediction(RecDat* rec_dat) override;
    void write(ostream& file) override;
    void read(istream& file) override;
    void clear() override;
    virtual ~AsymmetricFactorModel() {}
    bool self_test(){
      bool ok = Model::self_test();
      if(initialize_all_ and max_item_==-1){
        ok = false;
        cerr << "AsymmetricFactorModel::initialize_all_ is not used properly." << endl;
      }
      //if(begin_min_ == -1 and begin_max_ == -1){ //TODO
      //  ok = false;
      //  cerr << "FactorModel::begin_min_ and FactorModel::begin_max_ is not set properly." << endl;
      //}
      if(dimension_ <= 0){
        ok = false;
        cerr << "AsymmetricFactorModel::dimension_ <= 0." << endl;
      }
      if(norm_type_!="disabled" and norm_type_!="constant" and norm_type_!="recency" and norm_type_!="exponential" and norm_type_!="youngest"){ ok=false; cerr << "AsymmetricFactorModel::norm_type is not properly set, it should be one of the following: disabled, constant, recency, exponential, youngest." << endl; }
      return ok;
    }
  protected:
    void set_parameters(AsymmetricFactorModelParameters* parameters);
    void compute_user_factor(RecDat* rec_dat);
    double compute_norm(int user_activity_size);
    bool cache_is_valid(RecDat* rec_dat);
    vector<double> compute_histvector_sum(RecDat* rec_dat, const vector<const RecDat*>* user_history);

    //parameters
    const int dimension_;
    const bool initialize_all_, use_sigmoid_;
    const int max_item_;
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

#endif
