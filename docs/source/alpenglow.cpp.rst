alpenglow.cpp package
=====================

The classes in this module are usually not used directly, but instead through the :py:class:`alpenglow.Getter` class. For more info, read

- :doc:`/general/implement_your_model`
- :doc:`/general/cpp_api`
- :doc:`/general/simulation_attributes`
- :doc:`/general/memory_management`

Note that there are some C++ classes that have no python interface.  These are not documented here.

Filters
-------

The function of the filter interface is limiting the available set of items.  Current filters are whitelist-type filters, implementing :py:class:`alpenglow.cpp.WhitelistFilter`.

To use a filter in an experiment, wrap the model into the filter using :py:class:`alpenglow.cpp.WhitelistFilter2ModelAdapter`.

Example:

.. code-block:: python
  :emphasize-lines: 8-13

  class LabelExperiment(prs.OnlineExperiment):
      '''Sample experiment illustrating the usage of LabelFilter. The
      experiment contains a PopularityModel and a LabelFilter.'''
      def _config(self, top_k, seed):
          model = ag.PopularityModel()
          updater = ag.PopularityModelUpdater()
          updater.set_model(model)
          label_filter = ag.LabelFilter(**self.parameter_defaults(
              label_file_name = ""
          ))
          adapter = ag.WhitelistFilter2ModelAdapter()
          adapter.set_model(model)
          adapter.set_whitelist_filter(label_filter)


.. autoclass:: alpenglow.cpp.WhitelistFilter
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.WhitelistFilter2ModelAdapter
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.LabelFilterParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.LabelFilter
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.AvailabilityFilter
    :members:
    :undoc-members:
    :show-inheritance:

Offline evaluators
------------------

Use offline evaluators in traditional, fixed train/test split style learning.
Check the code of :py:class:`alpenglow.offline.OfflineModel.OfflineModel`
descendants for usage examples.



.. autoclass:: alpenglow.cpp.PrecisionRecallEvaluatorParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PrecisionRecallEvaluator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OfflineRankingComputerParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OfflinePredictions
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OfflineRankingComputer
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OfflineEvaluator
    :members:
    :undoc-members:
    :show-inheritance:

Recommender data
----------------

This module contains the classes that are responsible for reading in the
dataset and serving it to other classes of the experiment.

Interface :py:class:`alpenglow.cpp.RecommenderData` is the anchestor for
classes that read in the dataset.  The two most frequently used implementations
are :py:class:`alpenglow.cpp.DataframeData` and
:py:class:`alpenglow.cpp.LegacyRecommenderData`.

Interface :py:class:`alpenglow.cpp.RecommenderDataIterator` is the anchestor
for classes that serve the data to the classes in the online experiment.  See
:doc:`/general/anatomy_of_experiment` for general information.  The most
frequently used implementations are :py:class:`alpenglow.cpp.ShuffleIterator`
and :py:class:`alpenglow.cpp.SimpleIterator`.



.. autoclass:: alpenglow.cpp.RandomOnlineIteratorParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RandomOnlineIterator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ShuffleIteratorParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ShuffleIterator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.DataframeData
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SimpleIterator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RecommenderDataIterator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FactorRepr
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.UserItemFactors
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FactorModelReader
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.EigenFactorModelReader
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RandomIteratorParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RandomIterator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.InlineAttributeReader
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RecDat
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RecPred
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RecommenderData
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.LegacyRecommenderDataParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.LegacyRecommenderData
    :members:
    :undoc-members:
    :show-inheritance:

Utils
-----

This module contains miscellaneous helper classes.



.. autoclass:: alpenglow.cpp.PeriodComputerParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PeriodComputer
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SpMatrix
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.Random
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RankComputerParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RankComputer
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.Bias
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SparseAttributeContainerParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SparseAttributeContainer
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FileSparseAttributeContainer
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.Recency
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PowerLawRecencyParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PowerLawRecency
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PopContainer
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.TopPopContainer
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ToplistCreatorParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ToplistCreator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ToplistCreatorGlobalParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ToplistCreatorGlobal
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ToplistCreatorPersonalizedParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ToplistCreatorPersonalized
    :members:
    :undoc-members:
    :show-inheritance:

Gradient computers
------------------

This module contains the gradient computer classes that implement gradient
computation necessary in gradient methods.  See
:py:class:`alpenglow.experiments.FactorExperiment` for an example.



.. autoclass:: alpenglow.cpp.GradientComputer
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.GradientComputerPointWise
    :members:
    :undoc-members:
    :show-inheritance:

Objectives
----------

This module contains the implementation of objective functions that are
necessary for gradient computation in gradient learning methods.  See
:py:class:`alpenglow.experiments.FactorExperiment` for a usage example.



.. autoclass:: alpenglow.cpp.ObjectiveMSE
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ObjectivePointWise
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ObjectivePairWise
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ObjectiveListWise
    :members:
    :undoc-members:
    :show-inheritance:

General interfaces
------------------

This module contains the general interfaces that are implemented by classes
belonging to different modules.



.. autoclass:: alpenglow.cpp.Initializable
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.Updater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.NeedsExperimentEnvironment
    :members:
    :undoc-members:
    :show-inheritance:

Negative sample generators
--------------------------

All the samples in an implicit dataset are positive samples.  To make gradient
methods work, we need to provide negative samples too.  This module contains
classes that implement different negative sample generation algorithms.  These
classes implement :py:class:`alpenglow.cpp.NegativeSampleGenerator`.  The most
frequently used implementation is
:py:class:`alpenglow.cpp.UniformNegativeSampleGenerator`.



.. autoclass:: alpenglow.cpp.UniformNegativeSampleGeneratorParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.UniformNegativeSampleGenerator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.NegativeSampleGenerator
    :members:
    :undoc-members:
    :show-inheritance:

Offline learners
----------------

Use offline learners in traditional, fixed train/test split style learning.
Check the code of :py:class:`alpenglow.offline.OfflineModel.OfflineModel`
descendants for usage examples.



.. autoclass:: alpenglow.cpp.OfflineIteratingOnlineLearnerWrapperParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OfflineIteratingOnlineLearnerWrapper
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OfflineEigenFactorModelALSLearnerParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OfflineEigenFactorModelALSLearner
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OfflineLearner
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OfflineExternalModelLearnerParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OfflineExternalModelLearner
    :members:
    :undoc-members:
    :show-inheritance:

Loggers
-------

Loggers implement evaluators, statistics etc. in the online experiment.  These
classes implement interface :py:class:`alpenglow.cpp.Logger`.  See
:doc:`/general/anatomy_of_experiment` for a general view.



.. autoclass:: alpenglow.cpp.OnlinePredictions
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PredictionLogger
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RankingLog
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RankingLogs
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.MemoryRankingLoggerParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.MemoryRankingLogger
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ProceedingLogger
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.TransitionModelLoggerParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.TransitionModelLogger
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.Logger
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OnlinePredictorParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OnlinePredictor
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.MemoryUsageLogger
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.InterruptLogger
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ConditionalMetaLogger
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ListConditionalMetaLoggerParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ListConditionalMetaLogger
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.InputLoggerParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.InputLogger
    :members:
    :undoc-members:
    :show-inheritance:

Online experiment
-----------------

The central classes of the online experiments.



.. autoclass:: alpenglow.cpp.OnlineExperimentParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.OnlineExperiment
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ExperimentEnvironment
    :members:
    :undoc-members:
    :show-inheritance:

Models
------

The prediction models in the experiments.  The model interface is
:py:class:`alpenglow.cpp.Model`.  See
:doc:`/general/rank_computing_optimization` about different evaluation methods.


Factor models
-------------

This module contains the matrix factorization based models.



.. autoclass:: alpenglow.cpp.EigenFactorModelParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.EigenFactorModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FactorModelUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FmModelUpdaterParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FmModelUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FmModelParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FmModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.AsymmetricFactorModelGradientUpdaterParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.AsymmetricFactorModelGradientUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SvdppModelUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FactorModelParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FactorModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.AsymmetricFactorModelUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SvdppModelGradientUpdaterParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SvdppModelGradientUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FactorModelGlobalRankingScoreIterator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SvdppModelParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SvdppModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FactorModelGradientUpdaterParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.FactorModelGradientUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.AsymmetricFactorModelParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.AsymmetricFactorModel
    :members:
    :undoc-members:
    :show-inheritance:

Baseline models
---------------

This submodule contains the simple baseline models like nearest neighbor or most popular.


.. autoclass:: alpenglow.cpp.TransitionProbabilityModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.NearestNeighborModelParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.NearestNeighborModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PopularityTimeFrameModelUpdaterParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PopularityTimeFrameModelUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PersonalPopularityModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PersonalPopularityModelUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PopularityModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.NearestNeighborModelUpdaterParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.NearestNeighborModelUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.TransitionProbabilityModelUpdaterParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.TransitionProbabilityModelUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PopularityModelUpdater
    :members:
    :undoc-members:
    :show-inheritance:

Model combination
-----------------

This module contains the models that combine other models.  The most frequently
used class is :py:class:`alpenglow.cpp.CombinedModel`.  See
:doc:`/general/combination` for a usage example.



.. autoclass:: alpenglow.cpp.ToplistCombinationModelParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ToplistCombinationModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.WeightedModelStructure
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.WMSUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RandomChoosingCombinedModelParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RandomChoosingCombinedModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.CombinedModelParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.CombinedModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RandomChoosingCombinedModelExpertUpdaterParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RandomChoosingCombinedModelExpertUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.Evaluator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.CombinedDoubleLayerModelGradientUpdaterParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.CombinedDoubleLayerModelGradientUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.TopListRecommender
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.MassPredictor
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.Model
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ExternalModelParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ExternalModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PythonModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PythonToplistModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PythonRankingIteratorModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SimilarityModel
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ModelGradientUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.ModelMultiUpdater
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.RankingScoreIteratorProvider
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.GlobalRankingScoreIterator
    :members:
    :undoc-members:
    :show-inheritance:

Data generators
---------------

The classes in this module are responsible for generating data subsets from the
past.  This is necessary for embedding offline models into the online
framework, that needs to be updated in a batch.  See
:py:class:`alpenglow.experiments.BatchFactorExperiment` for a usage example.



.. autoclass:: alpenglow.cpp.DataGenerator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SamplingDataGeneratorParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.SamplingDataGenerator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.CompletePastDataGenerator
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.TimeframeDataGeneratorParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.TimeframeDataGenerator
    :members:
    :undoc-members:
    :show-inheritance:

Online learners
---------------

This module contains classes that modifiy the learning process, e.g. delay the
samples or feed them in a batch into offline learning methods.


.. autoclass:: alpenglow.cpp.LearnerPeriodicDelayedWrapperParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.LearnerPeriodicDelayedWrapper
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PeriodicOfflineLearnerWrapperParameters
    :members:
    :undoc-members:
    :show-inheritance:


.. autoclass:: alpenglow.cpp.PeriodicOfflineLearnerWrapper
    :members:
    :undoc-members:
    :show-inheritance:
