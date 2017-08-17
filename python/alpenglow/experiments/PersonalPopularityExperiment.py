import alpenglow.Getter as rs
import alpenglow as prs


class PersonalPopularityExperiment(prs.OnlineExperiment):
    """Recommends the item that the user has watched the most so far;
    in case of a tie, it falls back to global popularity. Running this model in conjunction
    with **exclude_known** == True is not recommended.
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

        model = rs.PersonalPopularityModel()
        updater = rs.PersonalPopularityModelUpdater()
        updater.set_model(model)

        model = model
        learner = [updater]

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
