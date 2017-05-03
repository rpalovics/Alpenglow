class Updater{
  %TypeHeaderCode
  #include "online_experiment/OnlineExperiment.h"
  %End
  public:
    virtual void update(RecDat* rec_dat)=0;
    bool selfTest();
};
