import alpenglow.Getter as rs
import alpenglow as prs


class NearestNeighborExperiment(prs.OnlineExperiment):
    def _config(self, top_k, seed):
        model = rs.NearestNeighborModel(**self.parameter_defaults(
          gamma=0.8,
          norm="num",
          direction="forward",
          gamma_threshold=0,
          num_of_neighbors=10
        ))
        updater = rs.NearestNeighborModelUpdater(**self.parameter_defaults(
          compute_similarity_period=86400,
          period_mode="time-based"
        ))
        updater.set_model(model)

        model = model
        learner = [updater]
        filters = [model]

        return (model, updater, [model], [])