struct FmModelUpdaterParameters{
  %TypeHeaderCode
  #include "models/factor/FmModelUpdater.h"
  %End
  double learning_rate;
};
class FmModelUpdater : public Updater{
  %TypeHeaderCode
  #include "models/factor/FmModelUpdater.h"
  %End
  public:
    FmModelUpdater(FmModelUpdaterParameters *parameters) ;
    void update(RecDat* rec_dat) ;
    void set_model(FmModel* model);
    bool self_test();
};
