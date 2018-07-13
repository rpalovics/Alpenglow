
class PersonalPopularityModel : public Model{
  %TypeHeaderCode
  #include "models/baseline/PersonalPopularityModel.h"
  %End
  public:
    double prediction(RecDat* rec_dat);
};

