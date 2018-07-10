#ifndef NEAREST_NEIGHBOR_MODEL_UPDATER
#define NEAREST_NEIGHBOR_MODEL_UPDATER

//SIP_AUTOCONVERT

#include <iostream>
#include "../../recommender_data/macros.h"
#include "../ModelUpdater.h"
#include "../../general_interfaces/Updater.h"
#include "NearestNeighborModel.h"

struct NearestNeighborModelUpdaterParameters {
  int compute_similarity_period;
  string period_mode;
  NearestNeighborModelUpdaterParameters(){
    compute_similarity_period=-1;
    period_mode="";
  }
};
class NearestNeighborModelUpdater : public Updater {
public:
  NearestNeighborModelUpdater(NearestNeighborModelUpdaterParameters* params):
    compute_similarity_period_(params->compute_similarity_period),
    period_mode_(params->period_mode==""?"time-based":params->period_mode)
  {
    model_ = NULL;
    last_similarity_recompute_period_ = 0;
  }
  void update(RecDat* rec_dat) override;
  void message(UpdaterMessage message) override;
  void set_model(NearestNeighborModel* model){ model_ = model; }
  bool self_test(){
    bool ok = Updater::self_test();
    if(model_==NULL){ ok=false; cerr << "NearestNeighborModelUpdater::model is not set." << endl; }
    if(period_mode_!="off" and compute_similarity_period_<=0){ ok=false; cerr << "NearestNeighborModelUpdater::compute_similarity_period_ <= 0." << endl; }
    if(!(period_mode_=="time-based" or period_mode_=="samplenum-based" or period_mode_=="off")){ ok=false; cerr << "NearestNeighborModelUpdater::period_mode_ is not set." << endl; }
    if(period_mode_=="samplenum-based"){ cerr << "WARNING: NearestNeighborModelUpdater::period_mode_=samplenum-based is not yet implemented." << endl; }
    return ok;
  }
private:
  void recompute_similarity();
  NearestNeighborModel* model_;
  int last_similarity_recompute_period_;
  const int compute_similarity_period_;
  const string period_mode_;

  vector<vector<pair<USER,int>>> userlist_for_items_;

};

#endif
