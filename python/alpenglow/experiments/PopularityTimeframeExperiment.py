import alpenglow.Getter as rs
import alpenglow as prs


class PopularityTimeframeExperiment(prs.OnlineExperiment):
    """PopularityTimeframeExperiment(tau=86400)

    Time-aware version of PopularityModel, which only considers the last 
    **tau** time interval when calculating popularities. Note that the
    time window ends at the timestamp of the last updating sample. The
    model does not take into consideration the timestamp of the sample
    for that the prediction is computed.


    Parameters
    ----------
    tau : int
        The time amount to consider.
    """
    def _config(self, top_k, seed):
        model = rs.PopularityModel()
        updater = rs.PopularityTimeFrameModelUpdater(**self.parameter_defaults(
          tau=86400
        ))
        updater.set_model(model)

        return (model, updater, [], [])
