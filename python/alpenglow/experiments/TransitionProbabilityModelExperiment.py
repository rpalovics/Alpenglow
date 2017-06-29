import alpenglow.Getter as rs
import alpenglow as prs


class TransitionProbabilityModelExperiment(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameter_defaults(
            top_k=100,
            min_time=0,
            seed=0,
            out_file=None,
            filters=[],
            loggers=[],
        )

        model = rs.TransitionProbabilityModel()
        updater = rs.TransitionProbabilityModelUpdater(**self.parameter_defaults(
          filter_freq_updates=False,
          mode_="normal",
          label_transition_mode_=False,
          label_file_name_=""
        ))
        updater.set_model(model)
        learner = rs.SimpleLearner()
        learner.add_simple_updater(updater)
        learner.set_model(model)

        model = model
        learner = learner
        filters = [model]

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
