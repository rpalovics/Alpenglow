import alpenglow.Getter as rs
import alpenglow as prs


class NearestNeighborModelExperiment(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameter_defaults(
            top_k=100,
            min_time=0,
            seed=0,
            out_file=None,
            filters=[],
            loggers=[],
        )

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
        learner = rs.SimpleLearner()
        learner.add_simple_updater(updater)
        learner.set_model(model)

        model = model
        learner = learner
        filters = [model]

        return {
            'config': config,
            'model': model,
            'learner': learner,
            'filters': filters,
        }
