
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
  %Docstring
    Interface for updaters.

    Updater instances can be set into
    :py:class:`alpenglow.cpp.OnlineExperiment` using the
    `add_updater()` function. The :py:meth:`update` function is called for each sample.
  %End
  public:
    virtual ~Updater();
    virtual void update(RecDat* rec_dat)=0;
  %Docstring
      update(RecDat* rec_dat)
      Function called by :py:class:`alpenglow.cpp.OnlineExperiment` for each
      sample.
  %End
//    virtual void message(UpdaterMessage message);
    bool self_test();
  %Docstring
      Returns true.
  %End
};

