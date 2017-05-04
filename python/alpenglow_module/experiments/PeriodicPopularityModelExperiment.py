import alpenglow.Getter as rs
import alpenglow as prs


class PeriodicPopularityModelExperiment(prs.OnlineExperiment):
    def config(self, elems):
        proceeding_logger = rs.ProceedingLogger()
        proceeding_logger.set_data_iterator(elems['randomAccessIterator'])

        config = self.parameterDefaults(
            topK=100,
            minTime=0,
            seed=0,
            outFile=None,
            filters=[],
        )
        config['loggers'] = [proceeding_logger] if self.verbose else []

        model = rs.PopularityModel()
        updater = rs.PopularityModelUpdater()
        updater.setModel(model)

        simpleLearner = rs.SimpleLearner()
        simpleLearner.add_simple_updater(updater)
        simpleLearner.set_model(model)

        learner = rs.LearnerPeriodicDelayedWrapper(**self.parameterDefaults(
            period=86400,
            delay=86400
        ))
        learner.set_wrapped_learner(simpleLearner)

        model = model
        learner = learner

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
