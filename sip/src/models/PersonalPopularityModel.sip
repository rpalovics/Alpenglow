class PersonalPopularityModel : public PopularityModel {
  %TypeHeaderCode
  #include "models/PersonalPopularityModel.h"
  %End
  public:
    PersonalPopularityModel();
    ~PersonalPopularityModel();
    void add(RecDat * recDat);
    double prediction(RecDat * recDat);
};