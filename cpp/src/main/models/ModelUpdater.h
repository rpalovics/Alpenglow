#ifndef MODEL_GRADIENT_UPDATER_H
#define MODEL_GRADIENT_UPDATER_H

//SIP_AUTOCONVERT

#include <vector>
#include "../recommender_data/RecommenderData.h"
#include "../models/Model.h"
#include "../general_interfaces/Updater.h"

class ModelGradientUpdater {
  public:
    virtual ~ModelGradientUpdater(){}
    virtual void update(RecDat* rec_dat, double gradient) = 0;
    virtual void message(UpdaterMessage message);
    bool self_test(){
      bool OK = true;
      return OK;
    }
  private:
    virtual void beginning_of_updating_cycle(){}
    virtual void end_of_updating_cycle(){}
};

class ModelMultiUpdater {
  public:
    virtual void update(vector<RecDat>* rec_dats) = 0;
    bool self_test(){
      bool OK = true;
      return OK;
    }
};

#endif /* MODEL_GRADIENT_UPDATER_H */
