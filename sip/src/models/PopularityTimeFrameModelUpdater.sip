
class PopularityTimeFrameModelUpdater : public ModelSimpleUpdater {
  %TypeHeaderCode
  #include "models/PopularityTimeFrameModelUpdater.h"
  %End
  public:
    void set_model(PopularityTimeFrameModel * model_);
    void update(RecDat * rec_dat);
};
