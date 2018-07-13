
class PopularityModelUpdater : public Updater {
  %TypeHeaderCode
  #include "models/baseline/PopularityModelUpdater.h"
  %End
  public:
    PopularityModelUpdater();
    void set_model(PopularityModel* model);
    void update(RecDat* rec_dat);
    bool self_test();
};

