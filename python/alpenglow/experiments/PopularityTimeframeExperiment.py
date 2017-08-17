import alpenglow.Getter as rs
import alpenglow as prs


class PopularityTimeframeExperiment(prs.OnlineExperiment):
    """PopularityTimeframeExperiment(tau=86400)

    Time-aware version of PopularityModel, which only considers the last 
    **tau** time interval when calculating popularities.

    Parameters
    ----------
    tau : int
        The time amount to consider.
    """
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
        updater = rs.PopularityTimeFrameModelUpdater(**self.parameter_defaults(
          tau=86400
        ))
        updater.set_model(model)

        model = model
        learner = [updater]

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
