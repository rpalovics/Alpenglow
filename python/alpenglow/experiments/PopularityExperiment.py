import alpenglow.Getter as rs
import alpenglow as prs


class PopularityExperiment(prs.OnlineExperiment):
    """Recommends the most popular item from the set of items seen so far.
    """

    def _config(self, top_k, seed):
        model = rs.PopularityModel()
        updater = rs.PopularityModelUpdater()
        updater.set_model(model)

        return (model, updater, [], [])