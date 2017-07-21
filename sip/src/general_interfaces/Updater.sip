//enum class UpdaterMessage{
//  start_of_fit,
//  end_of_fit,
//  start_of_implicit_update_cycle,
//  end_of_implicit_update_cycle
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
