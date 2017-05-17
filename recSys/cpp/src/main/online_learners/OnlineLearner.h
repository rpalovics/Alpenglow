#ifndef ONLINELEARNER
#define ONLINELEARNER


#include "../models/Model.h"
#include "../updaters/ModelUpdater.h"
#include "../updaters/GroupUpdater.h"


class OnlineLearner {
  public:
    OnlineLearner(){ model_ = NULL; }
    virtual ~OnlineLearner(){}
    virtual void learn(RecDat* rec_dat)=0;
    virtual void add_simple_updater(ModelSimpleUpdater* model_updater) {
      model_simple_updaters_.push_back(model_updater);
    }
    virtual void add_gradient_updater(ModelGradientUpdater* model_updater) {
      model_gradient_updaters_.push_back(model_updater);
    }
    virtual void add_multi_updater(ModelMultiUpdater* model_updater) {
      model_multi_updaters_.push_back(model_updater);
    }
    virtual void set_model(Model* model){model_=model;}
    bool self_test(){ return true; }

    //DEPRECATED
    void addSimpleUpdater(ModelSimpleUpdater* model_updater){
      cerr << "OnlineLearner:addSimpleUpdater is DEPRECATED, use google code." << endl;
      add_simple_updater(model_updater);
    }
    void addGradientUpdater(ModelGradientUpdater* model_updater){
      cerr << "OnlineLearner:addGradientUpdater is DEPRECATED, use google code." << endl;
      add_gradient_updater(model_updater);
    }
    bool selfTest(){ return self_test(); } //TODO google_code_temporal
  protected:
    vector<ModelSimpleUpdater*> model_simple_updaters_;
    vector<ModelGradientUpdater*> model_gradient_updaters_;
    vector<ModelMultiUpdater*> model_multi_updaters_;
    Model* model_;
};

#endif
