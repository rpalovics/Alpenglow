import alpenglow.Getter as rs
import alpenglow as prs


class PersonalPopularityExperiment(prs.OnlineExperiment):
    """Recommends the item that the user has watched the most so far;
    in case of a tie, it falls back to global popularity. Running this model in conjunction
    with **exclude_known** == True is not recommended.
    """

    def _config(self, top_k, seed):
        model = rs.PersonalPopularityModel()
        updater = rs.PersonalPopularityModelUpdater()
        updater.set_model(model)

        return (model, updater, [], [])
