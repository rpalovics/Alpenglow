
class PopularityModelUpdater : ModelSimpleUpdater{
  %TypeHeaderCode
  #include "updaters/PopularityModelUpdater.h"
  %End
  public:
    PopularityModelUpdater();
    void setModel(PopularityModel * model_ /Transfer/);
    virtual void update(RecDat * recDat);
    bool selfTest();
};