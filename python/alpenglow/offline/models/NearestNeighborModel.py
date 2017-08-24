import alpenglow.Getter as rs
import alpenglow.offline


class NearestNeighborModel(alpenglow.offline.OfflineModel):
    """NearestNeighborModel(num_of_neighbors=10)

    Item based nearest neighbor.

    Parameters
    ----------
    num_of_neighbors : int
        Number of neighbors to consider.
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

        learner = rs.OfflineIteratingLearner(**self.parameter_defaults(
            seed=67439852,
        ))
        learner.set_model(model)
        learner.add_simple_updater(updater)
        learner.set_recommender_data(recommender_data)

        return (model, learner)