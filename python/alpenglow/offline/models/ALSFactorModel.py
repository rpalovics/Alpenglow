import alpenglow.Getter as rs
import alpenglow.offline


class ALSFactorModel(alpenglow.offline.OfflineModel):
    """ALSFactorModel(dimension=10,begin_min=-0.01,begin_max=0.01,number_of_iterations=3,regularization_lambda=0.0001,alpha=40,implicit=1)

    This class implements the well-known matrix factorization recommendation model [Koren2009]_
    and trains it using ALS and iALS [Hu2008]_.

    Parameters
    ----------
    dimension : int
        The latent factor dimension of the factormodel.
    begin_min : double
        The factors are initialized randomly, sampling each element uniformly from the interval (begin_min, begin_max).
    begin_max : double
        See begin_min.
    number_of_iterations : double
        Number of times to optimize the user and the item factors for least squares.
    regularization_lambda : double
        The coefficient for the L2 regularization term. See [Hu2008]_. This number is multiplied by the number of non-zero elements of the user-item rating matrix before being used, to achieve similar magnitude to the one used in traditional SGD.
    alpha : int
        The weight coefficient for positive samples in the error formula in the case of implicit factorization. See [Hu2008]_.
    implicit: int
        Whether to treat the data as implicit (and optimize using iALS) or explicit (and optimize using ALS).
    """


    def _fit(self, recommender_data, users, items, matrix):
        model = rs.EigenFactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            seed=67439852,
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
