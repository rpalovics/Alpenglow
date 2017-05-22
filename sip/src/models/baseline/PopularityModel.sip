class PopularityModel : Model{
  %TypeHeaderCode
  #include "models/baseline/PopularityModel.h"
  %End
  public:
    PopularityModel();
    ~PopularityModel();
    void add(RecDat * rec_dat);
    double prediction(RecDat * rec_dat);
};
