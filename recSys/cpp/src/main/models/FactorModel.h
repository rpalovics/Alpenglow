#ifndef FACTOR_MODEL
#define FACTOR_MODEL

//#include <stdlib.h>
//#include <stdio.h>
//#include <iostream>
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <math.h>
//#include <vector>
//#include <set>
//#include <map>
//#include <utility>

#include <tr1/unordered_map>
#include "../utils/Factors.h"
#include "../utils/Util.h"
#include "../utils/Bias.h"
#include "../utils/Recency.h"
#include "Model.h"
#include "SimilarityModel.h"
#include <gtest/gtest_prod.h>

using namespace std;


struct FactorModelParameters{
  int dimension;
  double begin_min, begin_max;
  bool use_sigmoid;
  bool use_item_bias, use_user_bias;
  bool initialize_all;
  int max_item, max_user;
  //DEPRECATED
  double beginMin, beginMax;
  bool initializeAll, useSigmoid;
  int maxItem, maxUser;
  FactorModelParameters(){ //setting all to jinjactor default value
    dimension=-1;begin_min=-1;begin_max=-1;
    use_sigmoid=false;use_item_bias=false;use_user_bias=false;
    initialize_all=false;max_item=-1;max_user=-1;
    //DEPRECATED
    beginMin=-1;beginMax=-1;useSigmoid=false;
    initializeAll=false;maxItem=-1;maxUser=-1;
  }
};

class FactorModel : public Model, public SimilarityModel {
  public:
    FactorModel(FactorModelParameters *parameters):
      dimension_(parameters->dimension),
      begin_min_(parameters->begin_min!=-1?parameters->begin_min:parameters->beginMin),
      begin_max_(parameters->begin_max!=-1?parameters->begin_max:parameters->beginMax),
      use_sigmoid_(parameters->use_sigmoid or parameters->useSigmoid),
      initialize_all_(parameters->initialize_all or parameters->initializeAll),
      max_user_(parameters->max_user!=-1?parameters->max_user:parameters->maxUser),
      max_item_(parameters->max_item!=-1?parameters->max_item:parameters->maxItem),
      use_item_bias_(parameters->use_item_bias),
      use_user_bias_(parameters->use_user_bias)
    {
      if(parameters->beginMin!=-1 or
          parameters->beginMax!=-1 or
          parameters->useSigmoid!=false or
          parameters->initializeAll!=false or
          parameters->maxItem!=-1 or
          parameters->maxUser!=-1){
        cerr << "FactorModel's CamelCase parameters are DEPRECATED, use google code." << endl;
      }
      set_parameters(parameters);
    };
    //~FactorModel() { delete userFactors; delete itemFactors; }
    void add(RecDat* rec_dat) override;
    double prediction(RecDat* rec_dat) override;
    double similarity(int item1, int item2) override;
    void write(ofstream& file) override;
    void read(ifstream& file) override;
    void clear() override;
    void set_user_recency(Recency *recency){user_recency_ = recency;}
    void set_item_recency(Recency *recency){item_recency_ = recency;}
    void init(){ clear(); }
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

    //DEPRECATED
    void setUserRecency(Recency *recency){
      cerr << "FactorModel::setUserRecency is DEPRECATED, use google code." << endl;
      set_user_recency(recency);
    }
    void setItemRecency(Recency *recency){
      cerr << "FactorModel::setItemRecency is DEPRECATED, use google code." << endl;
      set_item_recency(recency);
    }
    void setUserBias(Bias *bias){
      cerr << "FactorModel::setUserBias is DEPRECATED in favor of use_user_bias parameter." << endl;
      use_user_bias_ = true;
      if(initialize_all_){
        for(int user=0;user<=max_user_;user++) user_bias_.init(user);
      }
    }
    void setItemBias(Bias * bias){
      cerr << "FactorModel::setItemBias is DEPRECATED in favor of use_item_bias parameter." << endl;
      use_item_bias_ = true;
      if(initialize_all_){
        for(int item=0;item<=max_item_;item++) item_bias_.init(item);
      }
    }
  protected:
    //parameters
    const int dimension_;
    const double begin_min_;
    const double begin_max_;
    const bool use_sigmoid_;
    const bool initialize_all_;
    const int max_user_;
    const int max_item_;
    bool use_item_bias_; //DEPRECATED, shoud be const
    bool use_user_bias_; //DEPRECATED, should be const

    //state
    Factors user_factors_, item_factors_;
    Bias user_bias_, item_bias_;
    Recency *user_recency_, *item_recency_;

    //other
    //double userFactorMean();
    //double itemFactorMean();
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
    FRIEND_TEST(TestFactorModel, test);
    FRIEND_TEST(TestFactorModel, similarity);
    FRIEND_TEST(TestFactorModelFilter, testBasic);
    FRIEND_TEST(TestFactorModelFilter, testRecency);
    FRIEND_TEST(TestFactorModelFilter, testBias);
    FRIEND_TEST(TestFactorModelFilter, testSigmoid);
    FRIEND_TEST(TestFactorModelFilter, testAll);
};

#endif
