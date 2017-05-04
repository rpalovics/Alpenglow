class PersonalPopularityModelUpdater : public PopularityModelUpdater {
  %TypeHeaderCode
  #include "updaters/PersonalPopularityModelUpdater.h"
  %End
  public:
    PersonalPopularityModelUpdater();
    void setModel(PersonalPopularityModel * model_);
    virtual void update(RecDat * recDat);
    bool selfTest();
};
