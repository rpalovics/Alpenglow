#ifndef MODELUPDATER
#define MODELUPDATER

#include <vector>
#include "../recommender_data/RecommenderData.h"
#include "../models/Model.h"

class ModelUpdater {
public:
  virtual double objective(RecDat*) {return 0.0;}
  virtual ~ModelUpdater() {}
  bool self_test(){ return true; }
};


class ModelGradientUpdater: public ModelUpdater {
  public:
    virtual void update(RecDat * recDat, double gradient) = 0;
    virtual void beginning_of_updating_cycle(RecDat*){} //before the first update call for any gradient updater, the parameter is the positive sample
    virtual void end_of_updating_cycle(RecDat*){} //befejezodott a gradiens frissites egy userre egy pozitiv es a hozza tartozo negativ mintakra minden gradiens updaterre, the parameter is the positive sample
    bool self_test(){
      bool OK = ModelUpdater::self_test();
      return OK;
    }
};

class ModelSimpleUpdater: public ModelUpdater {
  public:
    virtual void update(RecDat * recDat) = 0;
    virtual void start_of_updating(RecDat*){} //csak a teljesen offline hivja meg
    virtual void end_of_updating(RecDat*){} //csak a teljesen offline hivja meg
    bool self_test(){
      bool OK = ModelUpdater::self_test();
      return OK;
    }
};

class ModelMultiUpdater: public ModelUpdater {
  public:
    virtual void update(vector<RecDat>* rec_dats) = 0;
    bool self_test(){
      bool OK = ModelUpdater::self_test();
      return OK;
    }
};

#endif
