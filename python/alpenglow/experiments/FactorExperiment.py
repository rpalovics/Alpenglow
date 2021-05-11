import alpenglow.components.FactorComponent
import alpenglow as prs


class FactorExperiment(prs.OnlineExperiment):
    """FactorExperiment(dimension=10,begin_min=-0.01,begin_max=0.01,learning_rate=0.05,regularization_rate=0.0,negative_rate=100)

    This class implements an online version of the well-known matrix factorization recommendation model [Koren2009]_
    and trains it via stochastic gradient descent. The model is able to train on implicit data
    using negative sample generation, see [X.He2016]_ and the **negative_rate** parameter.

    .. [Koren2009] Koren, Yehuda, Robert Bell, and Chris Volinsky. "Matrix factorization techniques for recommender systems." Computer 42.8 (2009).
    .. [X.He2016] X. He, H. Zhang, M.-Y. Kan, and T.-S. Chua. Fast matrix factorization for online recommendation with implicit feedback. In SIGIR, pages 549–558, 2016.

    Parameters
    ----------
    dimension : int
        The latent factor dimension of the factormodel.
    begin_min : double
        The factors are initialized randomly, sampling each element uniformly from the interval (begin_min, begin_max).
    begin_max : double
        See begin_min.
    learning_rate : double
        The learning rate used in the stochastic gradient descent updates.
    regularization_rate : double
        The coefficient for the L2 regularization term.
    negative_rate : int
        The number of negative samples generated after each update. Useful for implicit recommendation.
    """
    def _config(self, top_k, seed):
        component = alpenglow.components.FactorComponent(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
            learning_rate=0.05,
            regularization_rate=0.0,
            negative_rate=100,
            seed=67439852,
            filter_repeats=False
        ))
        component.build()
        model = component.get_object("model")
        negative_sample_generator = component.get_object("negative_sample_generator")

        return (model, [negative_sample_generator], [])
