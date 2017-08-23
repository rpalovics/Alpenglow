#ifndef PERIODIC_TIMEFRAME_IMPLICIT_GRADIENT_LEARNER
#define PERIODIC_TIMEFRAME_IMPLICIT_GRADIENT_LEARNER

#include "PeriodicImplicitGradientLearner.h"
#include "../general_interfaces/INeedExperimentEnvironment.h"

using namespace std;

struct PeriodicTimeframeImplicitGradientLearnerParameters : public PeriodicImplicitGradientLearnerParameters {
  int timeframe_length;
};

class PeriodicTimeframeImplicitGradientLearner : public PeriodicImplicitGradientLearner {
  public:
    PeriodicTimeframeImplicitGradientLearner(PeriodicTimeframeImplicitGradientLearnerParameters* params):PeriodicImplicitGradientLearner(params){
      local_items_ = NULL;
      local_train_matrix_ = NULL;
      timeframe_length_ = params->timeframe_length;
    } 
    void set_local_items(vector<int>* items){ local_items_=items; }
    void set_local_train_matrix(SpMatrix* train_matrix){ local_train_matrix_=train_matrix; }
    bool self_test(){
      bool ok = PeriodicImplicitGradientLearner::self_test();
      if(timeframe_length_<=0){ ok=false; cerr << "PeriodicTimeframeImplicitGradientLearner::timeframe_length_ is not set." << endl; }
      if((local_train_matrix_==NULL and local_items_!=NULL) or (local_items_==NULL and local_train_matrix_!=NULL)){ cerr << "W ARNING: PeriodicTimeframeImplicitGradientLearner::local negative sample generation preferences are inconsistent. Set local_train_matrix AND local_items_ to use local negative sample generation. Running the learner in global negsample mode." << endl; }
      return ok;
    }
  private:
    SpMatrix* local_train_matrix_=NULL;
    vector<int>* local_items_=NULL;
    int timeframe_length_;
    vector<RecDat*> generate_train_data(RecDat* rec_dat) override;
    void initialize_model(RecDat* rec_dat, vector<RecDat*> train_data) override;
};

#endif
