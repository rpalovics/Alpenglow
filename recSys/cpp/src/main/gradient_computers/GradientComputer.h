#ifndef GRADIENT_COMPUTER
#define GRADIENT_COMPUTER

#include <iostream>
#include "../recommender_data/RecommenderData.h"
#include "../objectives/Objective.h"
#include "../models/Model.h"
#include "../models/ModelUpdater.h"
#include "../general_interfaces/Updater.h"

using namespace std;

class GradientComputer : public Updater{
  public:
    void set_model(Model* model){model_=model;}
    void add_gradient_updater(ModelGradientUpdater* gradient_updater){ 
      gradient_updaters_.push_back(gradient_updater);
    }
    void message(UpdaterMessage message){
      for(auto updater:gradient_updaters_){
        updater->message(message);
      }
    }
    bool self_test(){
      bool ok=Updater::self_test();
      if(model_==NULL){ ok=false; cerr << "GradientComputer::model is not set." << endl; }
      return ok;
    }
    //???
    virtual void set_up(vector<RecDat>* train_data) = 0;
    virtual vector<pair<RecDat*,double>>* get_next_gradient() = 0;
    virtual bool has_next() = 0;

  protected: 
    Model* model_ = NULL;
    vector<ModelGradientUpdater*> gradient_updaters_;
    //???
    vector<pair<RecDat*,double> > gradient_vector;
    vector<RecDat>* train_data;
};

class GradientComputerPointWise : public GradientComputer{
  public:
    void set_objective(ObjectivePointWise* objective){
      objective_ = objective;
    }
    void update(RecDat* rec_dat){
      model_->add(rec_dat);
      double gradient=get_gradient(rec_dat);
      for(auto updater:gradient_updaters_) updater->update(rec_dat,gradient); 
    }
    double get_gradient(RecDat* rec_dat); //TODO should be private
    bool self_test(){
      bool ok = GradientComputer::self_test();
      if(objective_==NULL) {ok=false;cerr << "GradientComputerPointWise::objective is not set." << endl; }
      return ok;
    }
    //???
    void set_up(vector<RecDat>* train_data_) override {
      train_data = train_data_;
      train_data_it = train_data->begin();
    }
    vector<pair<RecDat*,double>>* get_next_gradient() override;
    bool has_next() override {return (train_data_it!=train_data->end());}
  protected:
    ObjectivePointWise* objective_ = NULL;
    vector<RecDat>::iterator train_data_it;

};

class GradientComputerImplicitPairWise : public GradientComputer{
  public:
    GradientComputerImplicitPairWise(){}
    void set_objective(ObjectivePairWise* objective){
      objective_ = objective;
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
    ObjectivePairWise* objective_;
    vector<RecDat>::iterator train_data_it;
    RecDat* positive_data;
};

class GradientComputerListWise : public GradientComputer{
  public:
    GradientComputerListWise(){
      has_next_ = false;
    };
    void set_objective(ObjectiveListWise* objective){
      objective_ = objective;
    }
    void set_up(vector<RecDat>* train_data_) override {
      train_data = train_data_;
      has_next_=true;
    }
    vector<pair<RecDat*,double>>* get_next_gradient() override;
    bool has_next() override {return has_next_;}
  protected:
    ObjectiveListWise* objective_;
    bool has_next_;
};

#endif
