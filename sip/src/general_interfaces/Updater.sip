
//enum class UpdaterMessage{
//  %TypeHeaderCode
//  #include "general_interfaces/Updater.h"
//  %End
//  start_of_offline_update, 
//  end_of_offline_update,
//  start_of_offline_iterations, 
//  end_of_offline_iterations,
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

