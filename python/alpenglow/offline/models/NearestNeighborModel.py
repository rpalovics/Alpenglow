import alpenglow.Getter as rs
import alpenglow.offline


class NearestNeighborModel(alpenglow.offline.OfflineModel):
    """NearestNeighborModel(num_of_neighbors=10)

    One of the earliest and most popular collaborative filtering algorithms in practice is the item-based nearest neighbor [Sarwar2001]_
    For these algorithms similarity scores are computed between item pairs based on the co-occurrence of the pairs in the preference of users. Non-stationarity of the data can be accounted for e.g. with the introduction of a time-decay [Ding2005]_ .

    Describing the algorithm more formally, let us denote by :math:`U_i` the set of users that visited item :math:`i`, by :math:`I_u` the set of items visited by user :math:`u`, and by :math:`s_{u i}` the index of item :math:`i` in the sequence of interactions of user :math:`u`. The frequency based similarity function is defined by :math:`sim(j,i) = \\frac{\sum_{u\in {U_j \cap U_i}} 1}{\left|U_j\\right|}`. The score assigned to item :math:`i` for user :math:`u` is :math:`score(u,i) = \sum_{j\in{I_u}} sim(j,i).`
    The model is represented by the similarity scores. Only the most similar items are stored for each item. When the prediction scores are computed for a particular user, all items visited by the user are considered.

    Parameters
    ----------
    num_of_neighbors : int
        Number of most similar items that will be stored in the model.
    """
    def _fit(self, recommender_data, users, items, matrix):
        model = rs.NearestNeighborModel(
            gamma=1,
            norm="off",
            direction="both",
            gamma_threshold=0,
            num_of_neighbors=self.parameter_default("num_of_neighbors", 10),
        )

        updater = rs.NearestNeighborModelUpdater(
            period_mode="off",
        )
        updater.set_model(model)

        learner = rs.OfflineIteratingOnlineLearnerWrapper(
            seed=254938879,
            number_of_iterations=0,
            shuffle=False,
        )
        learner.add_updater(updater)

        return (model, learner)
