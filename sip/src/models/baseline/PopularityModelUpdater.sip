
class PopularityModelUpdater : ModelSimpleUpdater{
  %TypeHeaderCode
  #include "models/baseline/PopularityModelUpdater.h"
  %End
  public:
    PopularityModelUpdater();
    void set_model(PopularityModel* model);
    bool self_test();
    virtual void update(RecDat* rec_dat);
};
