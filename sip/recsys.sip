%Module alpenglow.cpp

%DefaultDocstringFormat "deindented"

%Include std/string.sip
%Include std/vector.sip
%Include std/map.sip
%Include std/pair.sip

std::string __compiler;
std::string __stdlib;
%ModuleCode
	#include <ciso646>
	
	#if defined(_LIBCPP_VERSION)
		std::string __stdlib = "libc++";
	#elif defined(__GLIBCPP__) || defined(__GLIBCXX__)
		std::string __stdlib = "libstdc++";
	#elif defined(_MSC_VER)
		std::string __stdlib = "msvc";
	#else
		std::string __stdlib = "unknown";
	#endif

	#if defined(__clang__)
		std::string __compiler = "clang";
	#elif defined(__GNUC__) || defined(__GNUG__)
		std::string __compiler = "gcc";
	#elif defined(_MSC_VER)
		std::string __compiler = "msvc";
	#elif defined(__MINGW32__)
		std::string __compiler = "mingw";
	#else
		std::string __compiler = "unknown";
	#endif
%End

%Include src/recommender_data/RecommenderData.sip
%Include src/recommender_data/RecommenderDataIterator.sip
%Include src/recommender_data/AttributeContainer.sip
%Include src/recommender_data/ShuffleIterator.sip
%Include src/recommender_data/RandomIterator.sip
%Include src/recommender_data/RandomOnlineIterator.sip
%Include src/recommender_data/SimpleIterator.sip
%Include src/recommender_data/DataframeData.sip
%Include src/recommender_data/FactorModelReader.sip
%Include src/models/Model.sip
%Include src/models/ModelUpdater.sip
%Include src/models/SimilarityModel.sip
%Include src/models/MassPredictor.sip
%Include src/models/ExternalModel.sip
%Include src/models/factor/FactorModel.sip
%Include src/models/factor/FactorModelGradientUpdater.sip
%Include src/models/factor/AsymmetricFactorModel.sip
%Include src/models/factor/AsymmetricFactorModelUpdater.sip
%Include src/models/factor/AsymmetricFactorModelGradientUpdater.sip
%Include src/models/factor/SvdppModel.sip
%Include src/models/factor/SvdppModelUpdater.sip
%Include src/models/factor/SvdppModelGradientUpdater.sip
%Include src/models/factor/EigenFactorModel.sip
%Include src/models/factor/FmModel.sip
%Include src/models/factor/FmModelUpdater.sip
%Include src/models/baseline/PopularityModel.sip
%Include src/models/baseline/PersonalPopularityModel.sip
%Include src/models/baseline/PersonalPopularityModelUpdater.sip
%Include src/models/baseline/TransitionProbabilityModel.sip
%Include src/models/baseline/TransitionProbabilityModelUpdater.sip
%Include src/models/baseline/PopularityModelUpdater.sip
%Include src/models/baseline/PopularityTimeFrameModelUpdater.sip
%Include src/models/baseline/NearestNeighborModel.sip
%Include src/models/baseline/NearestNeighborModelUpdater.sip
%Include src/models/combination/CombinedModel.sip
%Include src/models/combination/CombinedDoubleLayerModelGradientUpdater.sip
%Include src/models/combination/ToplistCombinationModel.sip
%Include src/models/combination/RandomChoosingCombinedModel.sip
%Include src/models/combination/RandomChoosingCombinedModelExpertUpdater.sip
%Include src/models/combination/Evaluator.sip
%Include src/models/combination/WeightedModelStructure.sip
%Include src/online_learners/LearnerPeriodicDelayedWrapper.sip
%Include src/online_learners/PeriodicOfflineLearnerWrapper.sip
%Include src/general_interfaces/Initializable.sip
%Include src/general_interfaces/NeedsExperimentEnvironment.sip
%Include src/general_interfaces/Updater.sip
%Include src/gradient_computers/GradientComputer.sip
%Include src/objectives/Objective.sip
%Include src/objectives/ObjectiveMSE.sip
%Include src/negative_sample_generators/NegativeSampleGenerator.sip
%Include src/negative_sample_generators/UniformNegativeSampleGenerator.sip
%Include src/data_generators/DataGenerator.sip
%Include src/data_generators/CompletePastDataGenerator.sip
%Include src/data_generators/SamplingDataGenerator.sip
%Include src/data_generators/TimeframeDataGenerator.sip
%Include src/loggers/Logger.sip
%Include src/loggers/ProceedingLogger.sip
%Include src/loggers/InterruptLogger.sip
%Include src/loggers/MemoryRankingLogger.sip
%Include src/loggers/OnlinePredictor.sip
%Include src/loggers/ConditionalMetaLogger.sip
%Include src/loggers/ListConditionalMetaLogger.sip
%Include src/loggers/PredictionLogger.sip
%Include src/loggers/InputLogger.sip
%Include src/loggers/MemoryUsageLogger.sip
%Include src/loggers/TransitionModelLogger.sip
%Include src/online_experiment/OnlineExperiment.sip
%Include src/online_experiment/ExperimentEnvironment.sip
%Include src/utils/SpMatrix.sip
%Include src/utils/PopContainers.sip
%Include src/utils/Recency.sip
%Include src/utils/Bias.sip
%Include src/utils/ToplistCreator.sip
%Include src/utils/Random.sip
%Include src/utils/PeriodComputer.sip
%Include src/utils/SparseAttributeContainer.sip
%Include src/utils/RankComputer.sip
%Include src/filters/ModelFilter.sip
%Include src/filters/AvailabilityFilter.sip
%Include src/filters/FactorModelFilter.sip
%Include src/filters/DummyModelFilter.sip


%Include src/offline_evaluators/OfflineEvaluator.sip
%Include src/offline_evaluators/PrecisionRecallEvaluator.sip
%Include src/offline_learners/OfflineLearner.sip
%Include src/offline_learners/OfflineIteratingOnlineLearnerWrapper.sip
%Include src/offline_learners/OfflineEigenFactorModelALSLearner.sip
%Include src/offline_learners/OfflineExternalModelLearner.sip
%Include src/offline_evaluators/OfflineRankingComputer.sip
