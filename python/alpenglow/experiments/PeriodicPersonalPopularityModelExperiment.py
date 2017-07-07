import alpenglow.Getter as rs
import alpenglow as prs


class PeriodicPersonalPopularityModelExperiment(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameter_defaults(
            top_k=100,
            min_time=0,
            seed=67439852,
            out_file=None,
            filters=[],
            loggers=[],
        )

        model = rs.PersonalPopularityModel()
        updater = rs.PersonalPopularityModelUpdater()
        updater.set_model(model)

        learner = rs.LearnerPeriodicDelayedWrapper(**self.parameter_defaults(
            period=86400,
            delay=86400
        ))
        learner.set_wrapped_learner(updater)

        model = model
        learner = [learner]

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
