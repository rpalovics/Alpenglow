
class ModelGradientUpdater {
  %TypeHeaderCode
  #include "models/ModelUpdater.h"
  %End
  public:
    virtual ~ModelGradientUpdater();
    virtual void update(RecDat * rec_dat, double gradient) = 0;
//    virtual void message(UpdaterMessage message);
    virtual void beginning_of_updating_cycle(RecDat*);
    virtual void end_of_updating_cycle(RecDat*);
    bool self_test();
};

class ModelMultiUpdater {
  %TypeHeaderCode
  #include "models/ModelUpdater.h"
  %End
  public:
    virtual void update(std::vector<RecDat>* rec_dats) = 0;
    bool self_test();
};

