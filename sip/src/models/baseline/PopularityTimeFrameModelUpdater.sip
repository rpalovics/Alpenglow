
struct PopularityTimeFrameModelUpdaterParameters {
  %TypeHeaderCode
  #include "models/baseline/PopularityTimeFrameModelUpdater.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.PopularityTimeFrameModelUpdater`. See documentation there.
  %End
  double tau;
};
class PopularityTimeFrameModelUpdater : public Updater {
  %TypeHeaderCode
  #include "models/baseline/PopularityTimeFrameModelUpdater.h"
  %End
  %Docstring
  Time-aware updater for PopularityModel, which only considers the last tau time interval when calculating popularities. Note that the time window ends at the timestamp of the last updating sample. the timestamp of the sample in the prediction call is not considered.
  %End
  public:
    PopularityTimeFrameModelUpdater(PopularityTimeFrameModelUpdaterParameters* params);
    void set_model(PopularityModel* model);
    void update(RecDat* rec_dat);
    bool self_test();
};

