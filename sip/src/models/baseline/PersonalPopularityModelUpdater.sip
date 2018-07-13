
class PersonalPopularityModelUpdater : public Updater{
  %TypeHeaderCode
  #include "models/baseline/PersonalPopularityModelUpdater.h"
  %End
  public:
    PersonalPopularityModelUpdater();
    void set_model(PersonalPopularityModel* model);
    void update(RecDat* rec_dat);
    bool self_test();
};

