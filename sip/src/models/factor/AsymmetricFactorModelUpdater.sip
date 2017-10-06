class AsymmetricFactorModelUpdater : public Updater{
  %TypeHeaderCode
  #include "models/factor/AsymmetricFactorModelUpdater.h"
  %End
  public:
    AsymmetricFactorModelUpdater();
    void update(RecDat* rec_dat);
    void set_model(AsymmetricFactorModel* model);
    bool self_test();
};
