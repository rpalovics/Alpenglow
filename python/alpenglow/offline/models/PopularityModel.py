import alpenglow.Getter as rs
import alpenglow.offline


class PopularityModel(alpenglow.offline.OfflineModel):
    """PopularityModel()

    Recommends the most popular item from the set of items.
    """
    def _fit(self, recommender_data, users, items, matrix):
        model = rs.PopularityModel()

        updater = rs.PopularityModelUpdater()
        updater.set_model(model)

        learner = rs.OfflineIteratingOnlineLearnerWrapper(
            seed=0,
            number_of_iterations=0,
            shuffle=False,
        )
        learner.add_updater(updater)

        return (model, learner)
