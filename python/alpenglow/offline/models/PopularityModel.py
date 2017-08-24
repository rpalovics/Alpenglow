import alpenglow.Getter as rs
import alpenglow.offline


class PopularityModel(alpenglow.offline.OfflineModel):
    """Recommends the most popular item from the set of items.
    """
    def _fit(self, recommender_data, users, items, matrix):
        model = rs.PopularityModel()

        updater = rs.PopularityModelUpdater()
        updater.set_model(model)

        learner = rs.OfflineIteratingLearner(**self.parameter_defaults(
            seed=67439852,
        ))
        learner.set_model(model)
        learner.add_simple_updater(updater)
        learner.set_recommender_data(recommender_data)

        return (model, learner)