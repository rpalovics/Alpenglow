Adjustable properties of evaluation
===================================

When running an online experiment, the user can control some modeling decisions and the flow of the experiment through global parameters.  These modeling decisions, control options and the parameters are described below.

Global properties of the online experiment
##########################################

The components in the online experiment can obtain the value of these parameters through query functions of :py:class:`alpenglow.cpp.ExperimentEnvironment`.  Note that some models ignore the value set in the common parameter container and always use the default value or a locally set value.

These are parameters of ::py:class:`alpenglow.OnlineExperiment` that affect the results of experiments.

+---------------------------------+---------------------------------------------+
| parameter name                  | description                                 |
+=================================+=============================================+
| ``exclude_known``               | Excludes items from evaluation that the     |
|                                 | actual user already interacted with.        |
|                                 | Besides evaluation, influences negative     |
|                                 | sample generation in gradient training.     |
|                                 | sample generation in gradient training. The |
|                                 | :code:`eval` columns of the input data      |
|                                 | should be set accordingly.                  |
+---------------------------------+---------------------------------------------+
| ``initialize_all``              | Set true to treat all users and items as    |
|                                 | existing from the beginning of the          |
|                                 | experiment.  Technically, the largest user  |
|                                 | and item is searched in the time series and |
|                                 | all ids starting from 0 will be treated as  |
|                                 | existing. By default this parameters is set |
|                                 | to false, meaning that users and items      |
|                                 | come into existence by their first          |
|                                 | occurrence in a training sample.            |
+---------------------------------+---------------------------------------------+
| ``top_k``                       | Sets the toplist length.  Models may treat  |
|                                 | scores liberally that belong to items that  |
|                                 | are under the limit to optimize running     |
|                                 | time.                                       |
+---------------------------------+---------------------------------------------+
| ``evaluation_start_time``       | Do not evaluate samples having smaller      |
|                                 | timestamp.                                  |
+---------------------------------+---------------------------------------------+
| ``experiment_termination_time`` | Terminates experiment after the first       |
|                                 | sample having equal or larger time stamp.   |
+---------------------------------+---------------------------------------------+


Other possibilities to control evaluation
#########################################

The eval column
+++++++++++++++
This field controls which datapoints are to be evaluated before training. Defaults to True for all samples. It should be set in accordance with the ``exclude_known`` parameter of the experiment.

Calculating toplists
++++++++++++++++++++
By default, Alpenglow doesn't actually compute toplists, see :ref:`ranking_optimization`. However it is still possible to actually calculate them using the ``calculate_toplists`` parameter of the online experiment. If simply ``True`` then all of the toplists are calculated. The other possibility is to provide a list of boolean values, specifying which training instances are the toplists to be calculated for.

The toplists themselves can be retrieved after the end of the run using :py:meth:`alpenglow.OnlineExperiment.OnlineExperiment.get_predictions`.

Filtering available items
+++++++++++++++++++++++++

It is possible to filter the evaluation to only consider a certain whitelist-set of available items at any time point. This can be useful for usecases such as TV recommendation, when not all items are available all the time.

For this, you'll need to configure your experiment and include an ``AvailabilityFilter``. For this, please refer to :py:class:`alpenglow.utils.AvailabilityFilter` and :py:class:`alpenglow.OnlineExperiment`.

Delaying training
+++++++++++++++++

In a real system the engine might not be capable of processing every item immediately after it arrives. To better simulate this, it is possible to delay the training, i.e. evaluate each item with a 1 hour old model. This is done by desynchronizing the training and evaluation timelines by wrapping the updater of the experiment in a delay wrapper.

For this, you'll need to configure your experiment and wrap your updater in a :py:class:`alpenglow.cpp.LearnerPeriodicDelayedWrapper`. This class is capable of simple delayed online updates and also delayed periodic updates, based on the values of the ``delay`` and ``period`` parameters.