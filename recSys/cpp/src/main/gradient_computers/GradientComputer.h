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
    virtual vector<pair<RecDat*,double>>* get_next_gradient() = 0;
    virtual bool has_next() = 0;

  protected: 
    Model* model;
    vector<pair<RecDat*,double> > gradient_vector;
    vector<RecDat>* train_data;
};

class GradientComputerPointWise : public GradientComputer{
  public:
    //GradientComputerPointWise(ObjectivePointWise * _objective){
    //  objective = _objective;
    //};
    GradientComputerPointWise(){}
    void set_objective(ObjectivePointWise* _objective){
      objective = _objective;
    }
    void set_up(vector<RecDat>* train_data_) override {
      train_data = train_data_;
      train_data_it = train_data->begin();
    }
    vector<pair<RecDat*,double>>* get_next_gradient() override;
    bool has_next() override {return (train_data_it!=train_data->end());}
    double get_gradient(RecDat* rec_dat);
  protected:
    ObjectivePointWise* objective;
    vector<RecDat>::iterator train_data_it;

};

class GradientComputerImplicitPairWise : public GradientComputer{
  public:
    //GradientComputerImplicitPairWise(ObjectivePairWise* _objective){
    //  objective = _objective;
    //};
    GradientComputerImplicitPairWise(){}
    void set_objective(ObjectivePairWise* _objective){
      objective = _objective;
    }
    void set_up(vector<RecDat>* train_data_) override {
      train_data=train_data_;
      train_data_it = train_data->begin();
      positive_data = &(*train_data_it);
      train_data_it++;
    }
    vector<pair<RecDat*,double>>* get_next_gradient() override;
    bool has_next() override {return (train_data_it!=train_data->end());}
  protected:
    ObjectivePairWise* objective;
    vector<RecDat>::iterator train_data_it;
    RecDat* positive_data;
};

class GradientComputerListWise : public GradientComputer{
  public:
    //GradientComputerListWise(ObjectiveListWise* _objective){
    //  objective = _objective;
    //  has_next_ = false;
    //};
    GradientComputerListWise(){
      has_next_ = false;
    };
    void set_objective(ObjectiveListWise* _objective){
      objective = _objective;
    }
    void set_up(vector<RecDat>* train_data_) override {
      train_data = train_data_;
      has_next_=true;
    }
    vector<pair<RecDat*,double>>* get_next_gradient() override;
    bool has_next() override {return has_next_;}
  protected:
    ObjectiveListWise* objective;
    bool has_next_;
};

#endif
