#ifndef ONLINE_EXPERIMENT_H
#define ONLINE_EXPERIMENT_H

//SIP_AUTOCONVERT

#include "../recommender_data/RecommenderDataIterator.h"
#include "../general_interfaces/Updater.h"
#include "../loggers/Logger.h"
#include "../general_interfaces/NeedsExperimentEnvironment.h"
#include "ExperimentEnvironment.h"

class ExperimentEnvironment;
class NeedsExperimentEnvironment;

struct OnlineExperimentParameters {
  int top_k = 100;
  int evaluation_start_time = 0;
  int experiment_termination_time = 0;
  bool exclude_known = true;
  bool initialize_all = false;
  int max_user = -1;
  int max_item = -1;
  int random_seed = 7992364;
};

class OnlineExperiment{
  /**
  The central class of the online experiment.
  
  It queries samples from the dataset, then one-by-one for each sample

  * calls loggers that are set using :py:meth:`add_logger`,
  * updates the environment and common statistics, see :py:class:`alpenglow.cpp.ExperimentEnvironment`,
  * calls the updaters that are set using :py:meth:`add_updater`.

  At the end of the experiment, it calls end loggers that are set using :py:meth:`add_end_logger`.

  //See anatomy_of_experiment for details. //TODO how to link?
  See :py:class:`alpenglow.OnlineExperiment.OnlineExperiment` for details.
  */
  public:
    OnlineExperiment(OnlineExperimentParameters* parameters);
    ~OnlineExperiment();
    void add_logger(Logger* logger){loggers_.push_back(logger);}
    /**
      add_logger(Logger* logger)
      Adds a logger instance.

      Parameters
      ----------
      logger : Logger*
          Pointer to the logger to be added.
    */
    void add_end_logger(Logger* logger){end_loggers_.push_back(logger);}
    /**
      add_end_logger(Logger* logger)
      Adds a logger instance, that will be called once at the end of the experiment.

      Parameters
      ----------
      logger : Logger*
          Pointer to the logger to be added.
    */
    void add_updater(Updater* updater){ updaters_.push_back(updater); }
    /**
      add_updater(Updater* updater)
      Adds an updater.

      Parameters
      ----------
      updater : Updater*
          Pointer to the updater to be added.
    */
    void set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator);
    /**
      set_recommender_data_iterator(RecommenderDataIterator* recommender_data_iterator)
      Sets the dataset of the experiment.

      Parameters
      ----------
      recommender_data_iterator : RecommenderDataIterator*
          Pointer to the dataset.
    */
    bool self_test(){
      bool ok = true;
      if(recommender_data_iterator_ == NULL){
        ok = false;
        cerr << "OnlineExperiment::recommender_data_iterator_ is not set." << endl;
      }
      if(loggers_.size() == 0){
        //ok = false; //That's not a fatal error.
        cerr << "WARNING: OnlineExperiment::loggers_ is empty." << endl;
      }
      return ok;
    }
    /**
      Tests if the dataset is set.

      Furthermore, the test produces a warning message if no loggers are set
      because in that case the the experiment will produce no output. 

      Returns
      -------
      bool
         Whether the tests were successful. 
    */
    void run(); //SIP_THROW (PythonKeyboardInterruptException) /ReleaseGIL/
    /**
      Runs the experiment.
    */
    void inject_experiment_environment_into(NeedsExperimentEnvironment* object);
    /**
      inject_experiment_environment_into(NeedsExperimentEnvironment* object)
      Sets the experiment environment into another object that requires it.

      In the online experiment, this method is automatically called with all the
      objects that implement :py:class:`alpenglow.cpp.NeedsExperimentEnvironment`,
      injecting the dependency where it is necessary. See the code of
      :py:class:`alpenglow.OnlineExperiment.OnlineExperiment` for details.
    */
  private:
    ExperimentEnvironment* experiment_environment_;
    RecommenderDataIterator* recommender_data_iterator_ = NULL;
    vector<Updater*> updaters_;
    vector<Logger*> loggers_;
    vector<Logger*> end_loggers_;
};

#endif /* ONLINE_EXPERIMENT_H */
