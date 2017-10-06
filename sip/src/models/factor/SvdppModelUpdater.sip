class SvdppModelUpdater : public Updater{
  %TypeHeaderCode
  #include "models/factor/SvdppModelUpdater.h"
  %End
  public:
    SvdppModelUpdater();
    void update(RecDat* rec_dat);
    void set_model(SvdppModel* model);
    bool self_test();
};
