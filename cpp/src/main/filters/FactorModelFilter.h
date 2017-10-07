#ifndef FACTORmodel_FILTER
#define FACTORmodel_FILTER

#include <algorithm>
#include "ModelFilter.h"
#include "../models/factor/FactorModel.h"
#include "../utils/Factors.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "../general_interfaces/Initializable.h"


class FactorFilter{
  public:
    FactorFilter(){
      factors_ = NULL;
      other_factors_ = NULL;
      entities_ = NULL;
      other_entities_ = NULL;
      upper_bounds_ = NULL;
    };
    ~FactorFilter(){};
    void set_factors(Factors* factors, Factors* other_factors);
    void set_entities(vector<int>* entities, vector<int>* other_entities);
    void set_upper_vector(vector<pair<int,double>>* upper_bounds){upper_bounds_=upper_bounds;}
    bool self_test(){
      bool OK = true;
      if(factors_==NULL){ OK=false; }
      if(other_factors_==NULL){ OK=false; }
      if(entities_==NULL){ OK=false; }
      if(other_entities_==NULL){ OK=false; }
      if(upper_bounds_==NULL){ OK=false; }
      if(!OK){cerr << "FactorFilter is not OK." << endl; }
      return OK;
    }
    void compute_bound_vectors();
    void compute_bounds();
    void run();
    vector<double>* get_upper(){return &upper_; }
    vector<double>* get_lower(){return &lower_; }
  private:
    void init_bounds(vector<double>* factor);
    void analyze_bounds(vector<double>* factor);
    void compute_upper_bounds();
    void compute_upper_bound(int entity);
    Factors *factors_, *other_factors_;
    vector<double> upper_, lower_;
    vector<int> *entities_, *other_entities_;
    vector<pair<int,double>>* upper_bounds_;   
};

class FactorModelFilter : public ModelFilter, public NeedsExperimentEnvironment, public Initializable {
 public:
   FactorModelFilter(){
     user_factor_filter_.set_upper_vector(&user_upper_bounds_);
     item_factor_filter_.set_upper_vector(&item_upper_bounds_);
     model_ = NULL;
     users_ = NULL;
     items_ = NULL;
   }
   void run(RecDat* rd) override;
   void run(double time){RecDat rd; rd.time=time; this->run(&rd);}
   vector<pair<int,double>>* get_global_users() override {return &user_upper_bounds_;}
   vector<pair<int,double>>* get_global_items() override {return &item_upper_bounds_;}
   void set_users(vector<int>* users);
   void set_items(vector<int>* items);
   void set_model(FactorModel* model);
   void set_experiment_environment(ExperimentEnvironment* experiment_environment) override {experiment_environment_=experiment_environment; }
   bool self_test(){
      bool OK = ModelFilter::self_test();
      if(!user_factor_filter_.self_test()){ OK=false; }
      if(!item_factor_filter_.self_test()){ OK=false; }
      if(model_==NULL){ OK=false; }
      if(users_==NULL){ OK=false; }
      if(items_==NULL){ OK=false; }
      if(!OK) cerr << "FactorModelFilter is not OK." << endl;
      return OK;
    }
 protected:
   bool autocalled_initialize() override {
     if(items_==NULL) items_=experiment_environment_->get_items();
     if(users_==NULL) users_=experiment_environment_->get_users();
     return true;
   }
 private:
   void compute_biases();
   void compute_bias(vector<pair<int,double> >* bounds, Bias& biases, vector<int>* entities, vector<pair<int,double> >* other_bounds);
   void compute_recencies(double time);
   void compute_recency(vector<pair<int,double> >*, Recency*, double);
   void compute_sigmoids();
   void compute_sigmoid(vector<pair<int,double> >*);
   FactorModel* model_;
   vector<int>* users_, *items_;
   ExperimentEnvironment* experiment_environment_;
   FactorFilter user_factor_filter_, item_factor_filter_;
   vector<pair<int,double>> user_upper_bounds_;   
   vector<pair<int,double>> item_upper_bounds_;   
};


#endif
