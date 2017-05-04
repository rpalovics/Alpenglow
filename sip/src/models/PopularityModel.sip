class PopularityModel : Model{
  %TypeHeaderCode
  #include "models/PopularityModel.h"
  %End
  public:
    PopularityModel();
    ~PopularityModel();
    void add(RecDat * recDat);
    double prediction(RecDat * recDat);
};