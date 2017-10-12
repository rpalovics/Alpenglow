import alpenglow.Getter as rs
import alpenglow.offline


class ALSFactorModel(alpenglow.offline.OfflineModel):
    def _fit(self, recommender_data, users, items, matrix):
        model = rs.EigenFactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            seed=254938879,
        ))
        learner = rs.OfflineEigenFactorModelALSLearner(**self.parameter_defaults(
            number_of_iterations=3,
            regularization_lambda=0.0001,
            alpha=40,
            implicit=1,
            clear_before_fit=1,
        ))
        learner.set_model(model)
        return (model, learner)
