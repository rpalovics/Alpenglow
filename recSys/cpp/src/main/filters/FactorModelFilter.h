#ifndef FACTORMODELFILTER
#define FACTORMODELFILTER

#include <algorithm>
#include "ModelFilter.h"
#include "../models/FactorModel.h"
#include "../utils/Factors.h"
#include "../utils/SortPairDescendingBySecond.h"


class FactorFilter{
  public:
    FactorFilter(){
      factors = NULL;
      otherFactors = NULL;
      entities = NULL;
      otherEntities = NULL;
      upperBounds = NULL;
    };
    ~FactorFilter(){};
    void setFactors(Factors * _factors, Factors * _otherFactors);
    void setEntities(vector <int> * _entities, vector <int> * _otherEntities);
    void setUpperVector(vector<pair<int,double> >* upperBounds_){upperBounds=upperBounds_;}
    bool self_test(){
      bool OK = true;
      if(factors==NULL){ OK=false; }
      if(otherFactors==NULL){ OK=false; }
      if(entities==NULL){ OK=false; }
      if(otherEntities==NULL){ OK=false; }
      if(upperBounds==NULL){ OK=false; }
      if(!OK){cerr << "FactorFilter is not OK." << endl; }
      return OK;
    }
    void computeBoundVectors();
    void computeBounds();
    void run();
    vector<double>* getUpper(){return &upper; }
    vector<double>* getLower(){return &lower; }
  private:
    void initBounds(vector <double> * factor);
    void analyzeBounds(vector <double> * factor);
    void computeUpperBounds();
    void computeUpperBound(int entity);
    Factors * factors, * otherFactors;
    vector <double> upper, lower;
    vector <int> * entities, * otherEntities;
    vector < pair <int, double> >* upperBounds;   
};

class FactorModelFilter : public ModelFilter{
 public:
   FactorModelFilter(){
     userFactorFilter = new FactorFilter;
     userFactorFilter->setUpperVector(&userUpperBounds);
     itemFactorFilter = new FactorFilter;
     itemFactorFilter->setUpperVector(&itemUpperBounds);
     model = NULL;
     users = NULL;
     items = NULL;
   };
   ~FactorModelFilter(){
     delete userFactorFilter;
     delete itemFactorFilter;
   };
    bool self_test(){
      bool OK = ModelFilter::self_test();
      if(!userFactorFilter->self_test()){ OK=false; }
      if(!itemFactorFilter->self_test()){ OK=false; }
      if(model==NULL){ OK=false; }
      if(users==NULL){ OK=false; }
      if(items==NULL){ OK=false; }
      return OK;
    }
   void run(double time);
   void run(RecDat* rd);
   vector<pair<int,double>>* get_global_users(){return &userUpperBounds;}
   vector<pair<int,double>>* get_global_items(){return &itemUpperBounds;}
   void setUsers(vector<int>* _users);
   void setItems(vector<int>* _items);
   void setModel(FactorModel * _model);
 private:
   void computeBiases();
   void computeBias(vector<pair<int,double> >* bounds, Bias& biases, vector<int>* entities, vector<pair<int,double> >* otherBounds);
   void computeRecencies(double time);
   void computeRecency(vector<pair<int,double> >*, Recency*, double);
   void computeSigmoids();
   void computeSigmoid(vector<pair<int,double> >*);
   FactorModel* model;
   vector<int>* users, *items;
   FactorFilter* userFactorFilter, *itemFactorFilter;
    vector<pair<int,double>> userUpperBounds;   
    vector<pair<int,double>> itemUpperBounds;   
};


#endif
