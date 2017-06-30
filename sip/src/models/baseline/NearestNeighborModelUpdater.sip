struct NearestNeighborModelUpdaterParameters {
  %TypeHeaderCode
  #include "models/baseline/NearestNeighborModelUpdater.h"
  %End
  int compute_similarity_period;
  std::string period_mode;
  NearestNeighborModelUpdaterParameters();
};

class NearestNeighborModelUpdater : public ModelSimpleUpdater {
  public:
  NearestNeighborModelUpdater(NearestNeighborModelUpdaterParameters* params);
  void update(RecDat* rec_dat);
  void end_of_updating(RecDat*);
  void set_model(NearestNeighborModel* model);
  bool self_test();
};
