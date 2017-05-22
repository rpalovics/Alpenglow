#ifndef FACTORMODELFILTER
#define FACTORMODELFILTER

#include <algorithm>
#include "ModelFilter.h"
#include "../models/factor/FactorModel.h"
#include "../utils/Factors.h"
#include "../utils/SortPairDescendingBySecond.h"


class FactorFilter{
  public:
    FactorFilter(){
      factors = NULL;
      other_factors = NULL;
      entities = NULL;
      other_entities = NULL;
      upper_bounds = NULL;
    };
    ~FactorFilter(){};
    void set_factors(Factors * _factors, Factors * _other_factors);
    void set_entities(vector <int> * _entities, vector <int> * _other_entities);
    void set_upper_vector(vector<pair<int,double> >* upper_bounds_){upper_bounds=upper_bounds_;}
    bool self_test(){
      bool OK = true;
      if(factors==NULL){ OK=false; }
      if(other_factors==NULL){ OK=false; }
      if(entities==NULL){ OK=false; }
      if(other_entities==NULL){ OK=false; }
      if(upper_bounds==NULL){ OK=false; }
      if(!OK){cerr << "FactorFilter is not OK." << endl; }
      return OK;
    }
    void compute_bound_vectors();
    void compute_bounds();
    void run();
    vector<double>* get_upper(){return &upper; }
    vector<double>* get_lower(){return &lower; }
  private:
    void init_bounds(vector <double> * factor);
    void analyze_bounds(vector <double> * factor);
    void compute_upper_bounds();
    void compute_upper_bound(int entity);
    Factors * factors, * other_factors;
    vector <double> upper, lower;
    vector <int> * entities, * other_entities;
    vector < pair <int, double> >* upper_bounds;   
};

class FactorModelFilter : public ModelFilter{
 public:
   FactorModelFilter(){
     user_factor_filter = new FactorFilter;
     user_factor_filter->set_upper_vector(&user_upper_bounds);
     item_factor_filter = new FactorFilter;
     item_factor_filter->set_upper_vector(&item_upper_bounds);
     model = NULL;
     users = NULL;
     items = NULL;
   };
   ~FactorModelFilter(){
     delete user_factor_filter;
     delete item_factor_filter;
   };
    bool self_test(){
      bool OK = ModelFilter::self_test();
      if(!user_factor_filter->self_test()){ OK=false; }
      if(!item_factor_filter->self_test()){ OK=false; }
      if(model==NULL){ OK=false; }
      if(users==NULL){ OK=false; }
      if(items==NULL){ OK=false; }
      return OK;
    }
   void run(double time);
   void run(RecDat* rd);
   vector<pair<int,double>>* get_global_users(){return &user_upper_bounds;}
   vector<pair<int,double>>* get_global_items(){return &item_upper_bounds;}
   void set_users(vector<int>* _users);
   void set_items(vector<int>* _items);
   void set_model(FactorModel * _model);
 private:
   void compute_biases();
   void compute_bias(vector<pair<int,double> >* bounds, Bias& biases, vector<int>* entities, vector<pair<int,double> >* other_bounds);
   void compute_recencies(double time);
   void compute_recency(vector<pair<int,double> >*, Recency*, double);
   void compute_sigmoids();
   void compute_sigmoid(vector<pair<int,double> >*);
   FactorModel* model;
   vector<int>* users, *items;
   FactorFilter* user_factor_filter, *item_factor_filter;
    vector<pair<int,double>> user_upper_bounds;   
    vector<pair<int,double>> item_upper_bounds;   
};


#endif
