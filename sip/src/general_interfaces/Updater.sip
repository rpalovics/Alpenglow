class Updater{
  %TypeHeaderCode
  #include "general_interfaces/Updater.h"
  %End
  public:
    virtual void update(RecDat* rec_dat)=0;
    virtual void start_of_updating(RecDat* rec_dat);
    virtual void end_of_updating(RecDat* rec_dat);
};
