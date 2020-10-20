
class ModelGradientUpdater {
  %TypeHeaderCode
  #include "models/ModelUpdater.h"
  %End
  public:
    virtual ~ModelGradientUpdater();
    virtual void update(RecDat * rec_dat, double gradient) = 0;
//    virtual void message(UpdaterMessage message);
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

