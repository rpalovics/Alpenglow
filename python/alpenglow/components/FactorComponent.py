import alpenglow as prs

class FactorComponent(prs.Component):
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

    def _get_catalog(self):
        catalog = {
          "model" : {
            "type" : "FactorModel",
            "object" : None,
            "parameters" : {
              "begin_min" : -0.01,
              "begin_max" : 0.01,
              "dimension" : 10,
              "initialize_all" : False
            },
            "connections" : []
          },
          "gradient_updater" : {
            "type" : "FactorModelGradientUpdater",
            "object" : None,
            "parameters" : {
              "learning_rate": 0.05,
              "regularization_rate" : 0.0
              },
            "connections" : [
              ("set_model", "model")
            ]
          },
          "point_wise_objective" : {
            "type" : "ObjectiveMSE",
            "object" : None,
            "parameters" : {},
            "connections" : []
          },
          "point_wise_gradient_computer" : {
            "type" : "GradientComputerPointWise",
            "object" : None,
            "parameters" : {},
            "connections" : [
              ("set_model", "model"),
              ("set_objective", "point_wise_objective"),
              ("add_gradient_updater", "gradient_updater")
            ]
          },
          "negative_sample_generator" : {
            "type" : "UniformNegativeSampleGenerator",
            "object" : None,
            "parameters" : {
              "negative_rate" : 100,
              "initialize_all" : False,
              "seed" : 67439852,
              "filter_repeats" : False
              },
            "connections" : [
              ("add_updater", "point_wise_gradient_computer")
            ]
          },
        }
        return catalog
