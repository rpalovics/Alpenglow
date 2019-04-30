
struct NearestNeighborModelUpdaterParameters {
  %TypeHeaderCode
  #include "models/baseline/NearestNeighborModelUpdater.h"
  %End
  %Docstring
  Constructor parameter struct for :py:class:`alpenglow.cpp.NearestNeighborModelUpdater`. See documentation there.
  %End
  int compute_similarity_period;
  std::string period_mode;
  NearestNeighborModelUpdaterParameters();
};
class NearestNeighborModelUpdater : public Updater {
  %TypeHeaderCode
  #include "models/baseline/NearestNeighborModelUpdater.h"
  %End
public:
  NearestNeighborModelUpdater(NearestNeighborModelUpdaterParameters* params);
  void update(RecDat* rec_dat);
//  void message(UpdaterMessage message);
  void set_model(NearestNeighborModel* model);
  bool self_test();
};

