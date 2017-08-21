import alpenglow.Getter as rs
import alpenglow as prs


class TransitionProbabilityExperiment(prs.OnlineExperiment):
    def _config(self, top_k, seed):
        model = rs.TransitionProbabilityModel()
        updater = rs.TransitionProbabilityModelUpdater(**self.parameter_defaults(
          filter_freq_updates=False,
          mode_="normal",
          label_transition_mode_=False,
          label_file_name_=""
        ))
        updater.set_model(model)

        return (model, updater, [model], [])