C++ API
=======

The core of Alpenglow is written in C++.
The C++ code is wrapped in python (see https://riverbankcomputing.com/software/sip/intro).
We instantiate the C++ objects from python and wire them together there.

Below we describe the most important C++ interfaces.

Online experiment
-----------------
Read :doc:`/general/anatomy_of_experiment` to get a general picture of the structure of the online experiments.  Here we describe the most important interfaces and they purpose in the online experiment.

Model
^^^^^

Interface: :py:class:`alpenglow.cpp.Model`

The recommender model of the experiment.
Even though the implementation of :py:class:`alpenglow.cpp.OnlineExperiment` does not specify the interface of the recommender model, all currently implemented models implement the :py:class:`alpenglow.cpp.Model` interface.
This interface provides function :py:meth:`alpenglow.cpp.Model.prediction` to query a score for a user-item pair.
The most commonly used evaluator, :py:class:`alpenglow.cpp.RankingLogger` that computes the rank of the correct item on the top list generated for the current user, expects interface ``Model``.
However, some models implement further interfaces like :py:class:`alpenglow.cpp.ToplistRecommender`, that let ``RankingLogger`` optimize rank computation.
See :doc:`/general/rank_computing_optimization` for details.

Updater
^^^^^^^

Interface: :py:class:`alpenglow.cpp.Updater`

In the online experiment, we iterate on a time series of samples.
The framework processes samples one by one.
For each sample, after the evaluation, in the training phase, the current sample becomes a training sample.

The central class of the experiment, :py:class:`alpenglow.cpp.OnlineExperiment` that manages the iteration on the samples, notifies the components in the experiment about the new sample through the interface :py:class:`alpenglow.cpp.Updater`.
It calls the function :py:meth:`alpenglow.cpp.Updater.update` for each sample.
Even though the updaters have access to all the samples from the past, most updaters use only the newest sample that they get through the update function.

The central class of the experiment accepts multiple ``Updater`` instances, and calls each of them during the training phase.
See :doc:`/general/anatomy_of_experiment/` for details.

Logger
^^^^^^

Interface: :py:class:`alpenglow.cpp.Logger`

In the online experiment, we iterate on a time series of samples.
The framework processes samples one by one.
For each sample, during the evaluation phase, the central class of the experiment, :py:class:`alpenglow.cpp.OnlineExperiment` that manages the iteration on the samples, calls :py:meth:`alpenglow.cpp.Logger.run` function of loggers that are set into it.  See :doc:`/general/anatomy_of_experiment/` for details.

Loggers can serve different purposes.  Their purpose can be to evaluate the experiment (see :py:class:`alpenglow.cpp.RankingLogger` as an example), log some info about the state of the experiment (e.g. :py:class:`alpenglow.cpp.MemoryUsageLogger`, :py:class:`alpenglow.cpp.ProceedingLogger`) or some statistics about the state of the recommender model (e.g. :py:class:`alpenglow.cpp.TransitionModelLogger`).

To log some data just before the termination of the experiment, set end loggers to the online experiment.  These loggers have to implement the same interface and be added to the central class of the online experiment using function :py:meth:`alpenglow.cpp.OnlineExperiment.add_end_logger`.  The central class calls :py:meth:`alpenglow.cpp.Logger.run` function of the end loggers after the training phase of the last sample is finished.  The parameter of the call is a ``NULL`` pointer.

RecommenderDataIterator
^^^^^^^^^^^^^^^^^^^^^^^

Interface: :py:class:`alpenglow.cpp.RecommenderDataIterator`.

The data must implement the interface :py:class:`alpenglow.cpp.RecommenderDataIterator`.
This class behaves like an iterator, but provides random access availability to the time series also.
The two most commonly used implementations, that are available in preconfigured experiments also are :py:class:`alpenglow.cpp.ShuffleIterator` and :py:class:`alpenglow.cpp.SimpleIterator`.
a
While the latter keeps the original order of the samples, the former shuffles the samples that have identical timestamp in order to get rid of any artificial order.
Use the parameter ``shuffle_same_time`` in the preconfigured experiments to choose the appropriate implementation.

Components for gradient based learning algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Updating gradient based recommenders require some common tasks independetly from the acutal algorithm.  These are described below together with the interfaces that are used to carry them out.

Negative sample generators
""""""""""""""""""""""""""

Interface: :py:class:`alpenglow.cpp.NegativeSampleGenerator`

In implicit datasets, normally all samples are positive samples.  Training gradient based recommenders using only positive samples would result in doubtful outcome.  To avoid this problem, we generate negative samples.  We treat all user-item pairs that are not present in the dataset as a negative sample.  The negative sample generators select from the set of these "missing" pairs using different strategies.

The simplest strategy is choosing uniformly randomly a fixed size set of items for the current user from the set of items that this user have not yet iteracted with.  This strategy is implemented in :py:class:`alpenglow.cpp.UniformNegativeSampleGenerator`.

In the implementation, the negative sample generators are present in the chain of the updaters.  They get the positive sample, generate negative ones and call to the next updater(s) for the original positive sample and for each negative one.  See :py:doc:`/general/anatomy_of_experiment` to learn more about the chain of the updaters.

Gradient computers and objectives
"""""""""""""""""""""""""""""""""

Interface: :py:class:`alpenglow.cpp.GradientComputer`, :py:class:`alpenglow.cpp.ObjectivePointWise`

In the alpenglow framework, the objective-dependent and model-dependent part of the gradient computation is separated, as much as this is (mathematically) possible.  The objective-depentent part is implemented in the gradient computer class, that passes the update call providing the gradient value to gradient updaters (see next section).

Gradient updaters
"""""""""""""""""

Interface: :py:class:`alpenglow.cpp.ModelGradientUpdater`

The gradient updater computes the model-dependent part of the gradient and updates the model.

General interfaces
------------------

These are administrative things, nothing to do with the recommender algorithm.  These make some administrative things, solved in a centralized way:

- injecting the common ``ExperimentEnvironment`` object into classes that require it (only in the online experiments),
- notify the classes about the end of the wiring phase,
- run self-checks to find wiring errors and faulty parameters.

In the preconfigured experiments (:py:mod:`alpenglow.experiments`, :py:mod:`alpenglow.offline`) these administration tasks are automatically performed.

NeedsExperimentEnvironment
^^^^^^^^^^^^^^^^^^^^^^^^^^

Interface: :py:class:`alpenglow.cpp.NeedsExperimentEnvironment`.

In the online experiment, the common data, centrally updated statistics and common simulation features are available to all objects through :py:class:`alpenglow.cpp.ExperimentEnvironment`.  The system can automatically inject this dependency to the objects using :py:meth:`alpenglow.Getter.MetaGetter.set_experiment_environment`.

In the offline experiments, ``ExperimentEnvironment`` is not available.  The common objects and parameters that would be available through it need to be set locally.

Initializable
^^^^^^^^^^^^^

Interface: :py:class:`alpenglow.cpp.Initializable`

The C++ objects are instantiated in python and then wired together using ``set_xxx()`` and ``add_xxx()`` functions.  When the wiring is finished, some object require a notification to make some initial tasks that depend on the final configuration (e.g. depend on the number of subobjects that were added).

Use :py:meth:`alpenglow.Getter.MetaGetter.initialize_all` to notify objects by calling :py:meth:`alpenglow.cpp.Initializable.initialize` when wiring is finished.

self_test() function
^^^^^^^^^^^^^^^^^^^^

Example: :py:meth:`alpenglow.cpp.FactorModel.self_test`

The wiring of the experiment is very error-prone.  Wiring errors may lead to segmentation faults and undefined behaviour.  To mitigate this problem, most of the classes can test themselves for missing subcomponents and contradictory parameters.  Use :py:meth:`alpenglow.Getter.MetaGetter.run_self_test` to call ``self_test`` for each object that implements this function.

Offline experiments
-------------------

The batch style experiments that have a fixed train-test split need some separate classes.  See :py:class:`alpenglow.cpp.OfflineLearner` and :py:class:`alpenglow.cpp.OfflineEvaluator`.

The models that are trained in batch style can be embedded in the online framework.  See :py:class:`alpenglow.experiments.ALSOnlineFactorExperiment` and :py:class:`alpenglow.experiments.BatchFactorExperiment`.  The embedding works in the other direction, see :py:class:`alpenglow.offline.models.PopularityModel`.
