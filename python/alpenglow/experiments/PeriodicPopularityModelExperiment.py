import alpenglow.Getter as rs
import alpenglow as prs


class PeriodicPopularityModelExperiment(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameter_defaults(
            top_k=100,
            min_time=0,
            seed=67439852,
            out_file=None,
            filters=[],
            loggers=[],
        )

        model = rs.PopularityModel()
        updater = rs.PopularityModelUpdater()
        updater.set_model(model)

        simple_learner = rs.SimpleLearner()
        simple_learner.add_simple_updater(updater)
        simple_learner.set_model(model)

        learner = rs.LearnerPeriodicDelayedWrapper(**self.parameter_defaults(
            period=86400,
            delay=86400
        ))
        learner.set_wrapped_learner(simple_learner)

        model = model
        learner = learner

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
