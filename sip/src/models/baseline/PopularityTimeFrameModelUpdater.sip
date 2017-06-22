struct PopularityTimeFrameModelUpdaterParameters {
  %TypeHeaderCode
  #include "models/baseline/PopularityTimeFrameModelUpdater.h"
  %End
  double tau;
};
class PopularityTimeFrameModelUpdater : public ModelSimpleUpdater {
  %TypeHeaderCode
  #include "models/baseline/PopularityTimeFrameModelUpdater.h"
  %End
  public:
    PopularityTimeFrameModelUpdater(PopularityTimeFrameModelUpdaterParameters*);
    void set_model(PopularityModel* model);
    void update(RecDat* rec_dat);
};
