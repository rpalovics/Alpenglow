
class PopularityModel : public Model {
  %TypeHeaderCode
  #include "models/baseline/PopularityModel.h"
  %End
  public:
    double prediction(RecDat* rec_dat);
};

