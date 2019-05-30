
class FactorModelUpdater : public Updater{
  %TypeHeaderCode
  #include "models/factor/FactorModelUpdater.h"
  %End
  public:
    void update(RecDat* rec_dat);
    void set_model(FactorModel* model);
    bool self_test();
};

