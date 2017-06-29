class Updater{
  %TypeHeaderCode
  #include "general_interfaces/Updater.h"
  %End
  public:
    virtual void update(RecDat* rec_dat)=0;
};
