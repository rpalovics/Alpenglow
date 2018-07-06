//enum class UpdaterMessage{
//  %TypeHeaderCode
//  #include "general_interfaces/Updater.h"
//  %End
//};
class Updater{
  %TypeHeaderCode
  #include "general_interfaces/Updater.h"
  %End
  public:
    virtual void update(RecDat* rec_dat)=0;
//    virtual void message(UpdaterMessage message);
    bool self_test();
};
