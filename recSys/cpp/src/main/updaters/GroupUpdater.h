#ifndef GROUP_UPDATER
#define GROUP_UPDATER

#include "ModelUpdater.h"
#include "../gradient_computers/GradientComputer.h"

class GroupUpdater{
  public:
    //static int update_gradient_updaters(GradientComputer* grc, vector<ModelGradientUpdater*>* upd){
    //  return update_gradient_updaters(NULL, grc, upd);
    //}
    static int update_gradient_updaters(RecDat* rec_dat, GradientComputer*, vector<ModelGradientUpdater*>*);
  private:
};

#endif
