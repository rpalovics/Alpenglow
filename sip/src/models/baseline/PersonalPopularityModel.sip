class PersonalPopularityModel : public Model{
  %TypeHeaderCode
  #include "models/baseline/PersonalPopularityModel.h"
  %End
  public:
    PersonalPopularityModel();
    ~PersonalPopularityModel();
    void add(RecDat* rec_dat);
    double prediction(RecDat* rec_dat);
};
