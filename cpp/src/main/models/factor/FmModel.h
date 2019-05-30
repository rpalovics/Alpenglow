#ifndef FM_MODEL_H
#define FM_MODEL_H

#include <unordered_map>
#include "../../utils/Factors.h"
#include "../../utils/SparseAttributeContainer.h"
#include "../Model.h"
#include "../SimilarityModel.h"

//SIP_AUTOCONVERT

using namespace std;

struct FmModelParameters {
  int dimension=10;
  double begin_min=-0.01;
  double begin_max=0.01;
  // bool use_item_bias, use_user_bias;
  int seed=745578;
  SparseAttributeContainer *user_attributes = NULL;
  SparseAttributeContainer *item_attributes = NULL;
};

class FmModel : public Model, public Initializable
{
  public:
    FmModel(FmModelParameters *parameters):
      dimension_(parameters->dimension),
      begin_min_(parameters->begin_min),
      begin_max_(parameters->begin_max),
      user_attributes_(parameters->user_attributes),
      item_attributes_(parameters->item_attributes),
      seed_(parameters->seed)
    {
      FactorsParameters factors_parameters;
      factors_parameters.begin_min=begin_min_;
      factors_parameters.begin_max=begin_max_;
      factors_parameters.dimension=dimension_;
      factors_parameters.seed=parameters->seed;
      user_factors_.set_parameters(factors_parameters);
      factors_parameters.seed+=1;
      item_factors_.set_parameters(factors_parameters);
    };
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat) override;
    void clear() override;
    bool self_test(){
      bool ok = Model::self_test();
      return ok;
    }
  protected:
    //parameters
    const int dimension_;
    const double begin_min_;
    const double begin_max_;
    const int seed_;

    //inner functionality
    vector<double> sum_attribute_vector(
      Factors &factors,
      const unordered_map<int, double> &attributes
    );
    void update_attributes(
      Factors &factors,
      const unordered_map<int, double> &attributes,
      vector<double> &gradient_direction,
      double gradient_multiplier
    );
    bool can_predict_(RecDat* rec_dat);
    const unordered_map<int, double>& get_attributes(SparseAttributeContainer *container, int id);

    //state
    Factors user_factors_, item_factors_;
    SparseAttributeContainer *user_attributes_ = NULL, *item_attributes_ = NULL;
    unordered_map<int, double> indentity_;
    vector<bool> seen_users_;
    vector<bool> seen_items_;

    //general methods
    bool autocalled_initialize() override;

    //friends
    friend class FmModelUpdater;
    FRIEND_TEST(TestFmModel, test_protected);
    FRIEND_TEST(TestFmModel, test_updater);
    FRIEND_TEST(TestFmModel, test);
};

#endif /* FM_MODEL_H */
