class PersonalPopularityModelUpdater : public PopularityModelUpdater {
  %TypeHeaderCode
  #include "updaters/PersonalPopularityModelUpdater.h"
  %End
  public:
    PersonalPopularityModelUpdater();
    void set_model(PersonalPopularityModel * model_);
    virtual void update(RecDat * recDat);
    bool selfTest();
};
