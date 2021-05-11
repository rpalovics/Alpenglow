import alpenglow.components.PopularityComponent
import alpenglow as prs


class PopularityExperiment(prs.OnlineExperiment):
    """Recommends the most popular item from the set of items seen so far.
    """

    def _config(self, top_k, seed):
        component = alpenglow.components.PopularityComponent()
        component.build()
        model = component.get_object("model")
        updater = component.get_object("updater")

        return (model, [updater], [])
