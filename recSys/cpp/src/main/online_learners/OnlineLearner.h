#ifndef ONLINELEARNER
#define ONLINELEARNER


#include "../general_interfaces/Updater.h"
#include "../models/Model.h"
#include "../models/ModelUpdater.h"
#include "../models/GroupUpdater.h"


class OnlineLearner : public Updater{
  public:
    OnlineLearner(){ model_ = NULL; }
    virtual ~OnlineLearner(){}
    void update(RecDat* rec_dat) override { learn(rec_dat); }
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
    bool self_test(){ return Updater::self_test(); }

  protected:
    vector<ModelSimpleUpdater*> model_simple_updaters_;
    vector<ModelGradientUpdater*> model_gradient_updaters_;
    vector<ModelMultiUpdater*> model_multi_updaters_;
    Model* model_;
};

#endif
