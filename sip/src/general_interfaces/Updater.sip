
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
    Updater of an associated :py:class:`alpenglow.cpp.Model` instance or other
    object of the simulation.

    :py:class:`alpenglow.cpp.OnlineExperiment` notifies objects about the
    progress of simulation time through the :py:meth:`update` function of this
    interface.
    :py:class:`alpenglow.cpp.OnlineExperiment` accepts `Updater` instances
    through its `add_updater()` function. During the online experiment,
    it calls the :py:meth:`update()` function for each sample.
  %End
  public:
    virtual ~Updater();
    virtual void update(RecDat* rec_dat)=0;
  %Docstring
      update(RecDat* rec_dat)

      Updates the associated model or other object of the simulation.

      Parameters
      ----------
      rec_dat : RecDat*
         The newest available sample of the experiment.
  %End
//    virtual void message(UpdaterMessage message);
    bool self_test();
  %Docstring
      Returns true.
  %End
};

