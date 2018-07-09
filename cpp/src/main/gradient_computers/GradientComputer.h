#ifndef GRADIENT_COMPUTER
#define GRADIENT_COMPUTER

//SIP_AUTOCONVERT

#include <iostream>
#include "../recommender_data/RecommenderData.h"
#include "../objectives/Objective.h"
#include "../models/Model.h"
#include "../models/ModelUpdater.h"
#include "../general_interfaces/Updater.h"

using namespace std;

//TODO one level?
class GradientComputer : public Updater{ //SIP_ABSTRACT
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

  protected: 
    Model* model_ = NULL;
    vector<ModelGradientUpdater*> gradient_updaters_;
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
    bool self_test(){
      bool ok = GradientComputer::self_test();
      if(objective_==NULL) {ok=false;cerr << "GradientComputerPointWise::objective is not set." << endl; }
      return ok;
    }
  private:
    ObjectivePointWise* objective_ = NULL;
    double get_gradient(RecDat* rec_dat);

};

//todo: update


// class GradientComputerImplicitPairWise : public GradientComputer{
//   public:
//     GradientComputerImplicitPairWise(){}
//     void set_objective(ObjectivePairWise* objective){
//       objective_ = objective;
//     }
//     void set_up(vector<RecDat>* train_data_) override {
//       train_data=train_data_;
//       train_data_it = train_data->begin();
//       positive_data = &(*train_data_it);
//       train_data_it++;
//     }
//     vector<pair<RecDat*,double>>* get_next_gradient() override;
//     bool has_next() override {return (train_data_it!=train_data->end());}
//   protected:
//     ObjectivePairWise* objective_;
//     vector<RecDat>::iterator train_data_it;
//     RecDat* positive_data;
// };

// class GradientComputerListWise : public GradientComputer{
//   public:
//     GradientComputerListWise(){
//       has_next_ = false;
//     };
//     void set_objective(ObjectiveListWise* objective){
//       objective_ = objective;
//     }
//     void set_up(vector<RecDat>* train_data_) override {
//       train_data = train_data_;
//       has_next_=true;
//     }
//     vector<pair<RecDat*,double>>* get_next_gradient() override;
//     bool has_next() override {return has_next_;}
//   protected:
//     ObjectiveListWise* objective_;
//     bool has_next_;
// };

#endif
