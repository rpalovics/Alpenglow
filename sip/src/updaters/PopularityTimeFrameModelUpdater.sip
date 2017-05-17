
class PopularityTimeFrameModelUpdater : public ModelSimpleUpdater {
  %TypeHeaderCode
  #include "updaters/PopularityTimeFrameModelUpdater.h"
  %End
  public:
    void set_model(PopularityTimeFrameModel * model_);
    void update(RecDat * recDat);
};
