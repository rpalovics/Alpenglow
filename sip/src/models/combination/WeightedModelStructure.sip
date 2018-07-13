
struct WeightedModelStructure{
  %TypeHeaderCode
  #include "models/combination/WeightedModelStructure.h"
  %End
  std::vector<Model*> models_;
  std::vector<double> distribution_;
  bool is_initialized();
};

class WMSUpdater{
  %TypeHeaderCode
  #include "models/combination/WeightedModelStructure.h"
  %End
public:
  virtual void set_wms(WeightedModelStructure* wms)=0;
};

