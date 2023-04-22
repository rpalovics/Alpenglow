import alpenglow as prs


class TransitionProbabilityExperiment(prs.OnlineExperiment):
    """TransitionProbabilityExperiment(mode="normal")

    A simple algorithm that focuses on the sequence of items a user has visited is one that records how often users visited item i after visiting another item j. This can be viewed as particular form of the item-to-item nearest neighbor with a time decay function that is non-zero only for the immediately preceding item. While the algorithm is more simplistic, it is fast to update the transition fre- quencies after each interaction, thus all recent information is taken into account.

    Parameters
    ----------
    mode : string
        The direction of transitions to be considered, possible values: normal, inverted, symmetric.
    """
    def _config(self, top_k, seed):
        component = prs.components.TransitionProbabilityComponent(
          parameter_container=self
        )
        component.build()
        model = component.get_object("model")
        updater = component.get_object("updater")

        return (model, updater, [])
