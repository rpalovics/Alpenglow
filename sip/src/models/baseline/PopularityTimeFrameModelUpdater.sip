
struct PopularityTimeFrameModelUpdaterParameters{
  %TypeHeaderCode
  #include "models/baseline/PopularityTimeFrameModelUpdater.h"
  %End
  double tau;
  PopularityTimeFrameModelUpdaterParameters();
};
class PopularityTimeFrameModelUpdater : public Updater {
  %TypeHeaderCode
  #include "models/baseline/PopularityTimeFrameModelUpdater.h"
  %End
  public:
    PopularityTimeFrameModelUpdater(PopularityTimeFrameModelUpdaterParameters* params);
    void set_model(PopularityModel* model);
    void update(RecDat* rec_dat);
    bool self_test();
};

