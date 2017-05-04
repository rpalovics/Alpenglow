#ifndef COMBINED_DOUBLE_LAYER_LEARNER
#define COMBINED_DOUBLE_LAYER_LEARNER

#include "OnlineLearner.h"
using namespace std;

/** CombinedDoubleLayerLearner
  * Main usage: CombinedModel + gradient learner for combination weights + learners for each model
  * Other usages: one model with multiple learners
  * parameters/setable/addable elements:
  * set_meta_learner: learner designated for combination weights, runs before other learners (optional)
  * add_learner: add other learners here
  */
class CombinedDoubleLayerLearner : public OnlineLearner{
  public:
    CombinedDoubleLayerLearner(){
      combination_learner_ = NULL;
    } 
    void add_learner(OnlineLearner* learner){learners_.push_back(learner);}
    void set_meta_learner(OnlineLearner* learner){combination_learner_ = learner;}
    void learn(RecDat* recDat) override;
    bool self_test(){
      bool ok = OnlineLearner::self_test();
      if(learners_.size()==0){ cerr << "W ARNING: CombinedDoubleLayerLearner::learners_ is empty." << endl; }
      if(model_simple_updaters_.size()!=0){ cerr << "W ARNING: CombinedDoubleLayerLearner::model_simple_updaters_ should be empty as they are not called by the learner." << endl; }
      if(model_gradient_updaters_.size()!=0){ cerr << "W ARNING: CombinedDoubleLayerLearner::model_gradient_updaters_ should be empty as they are not called by the learner." << endl; }
      return ok;
    }
    //DEPRECATED
    void addLearner(OnlineLearner* learner){cerr << "CombinedDoubleLayerLearner::addLearner is DEPRECATED, use google code." << endl; add_learner(learner);}
    void setMetaLearner(OnlineLearner* learner){cerr << "CombinedDoubleLayerLearner::setMetaLearner is DEPRECATED, use google code." << endl;set_meta_learner(learner);}
  private:
    OnlineLearner* combination_learner_;
    vector<OnlineLearner*> learners_;
};


#endif
