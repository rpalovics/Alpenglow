
class PopularityTimeFrameModelUpdater : public ModelSimpleUpdater {
  %TypeHeaderCode
  #include "updaters/PopularityTimeFrameModelUpdater.h"
  %End
  public:
    void setModel(PopularityTimeFrameModel * model_ /Transfer/);
    void update(RecDat * recDat);
};
