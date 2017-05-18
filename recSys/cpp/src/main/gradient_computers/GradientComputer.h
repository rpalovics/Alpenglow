#ifndef GRADIENT_COMPUTER
#define GRADIENT_COMPUTER

#include "../recommender_data/RecommenderData.h"
#include "../objectives/Objective.h"
#include "../models/Model.h"

using namespace std;

class GradientComputer{
  public:
    GradientComputer(){};
    virtual ~GradientComputer(){};
    void set_model(Model* model_){model=model_;}
    virtual void set_up(vector<RecDat>* train_data) = 0;
    virtual vector<pair<RecDat*,double> >* get_next_gradient() = 0;
    virtual bool has_next() = 0;

  protected: 
    Model* model;
    vector<pair<RecDat*,double> > gradientVector;
    vector<RecDat>* train_data;
};

class GradientComputerPointWise : public GradientComputer{
  public:
    GradientComputerPointWise(ObjectivePointWise * _objective){
      objective = _objective;
    };
    GradientComputerPointWise(){}
    void set_objective(ObjectivePointWise* _objective){
      objective = _objective;
    }
    ~GradientComputerPointWise(){};
    virtual double get_gradient(RecDat* rec_dat);
    void set_up(vector<RecDat>* train_data_){
      train_data = train_data_;
      train_data_it = train_data->begin();
    }
    vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next(){return (train_data_it!=train_data->end());}
  protected:
    ObjectivePointWise * objective;
    vector<RecDat>::iterator train_data_it;

};

class GradientComputerImplicitPairWise : public GradientComputer{
  public:
    GradientComputerImplicitPairWise(ObjectivePairWise * _objective){
      objective = _objective;
    };
    ~GradientComputerImplicitPairWise(){};
    void set_up(vector<RecDat>* train_data_){
      train_data=train_data_;
      train_data_it = train_data->begin();
      positiveData = &(*train_data_it);
      train_data_it++;
    }
    vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next(){return (train_data_it!=train_data->end());}
  protected:
    ObjectivePairWise * objective;
    vector<RecDat>::iterator train_data_it;
    RecDat* positiveData;
};

class GradientComputerListWise : public GradientComputer{
  public:
    GradientComputerListWise(ObjectiveListWise * _objective){
      objective = _objective;
      has_next_ = false;
    };
    ~GradientComputerListWise(){};
    void set_up(vector<RecDat>* train_data_){
      train_data = train_data_;
      has_next_=true;
    }
    vector<pair<RecDat*,double> > * get_next_gradient();
    bool has_next(){return has_next_;}
  protected:
    ObjectiveListWise * objective;
    bool has_next_;
};

#endif
