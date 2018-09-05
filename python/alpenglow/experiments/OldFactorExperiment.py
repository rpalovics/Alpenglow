import alpenglow.Getter as rs
import alpenglow as prs


class OldFactorExperiment(prs.OnlineExperiment):
    """OldFactorExperiment(dimension=10,begin_min=-0.01,begin_max=0.01,learning_rate=0.05,regularization_rate=0.0,negative_rate=0.0)

    Deprecated, use FactorExperiment.
    This class implements an online version of the well-known matrix factorization recommendation model [Koren2009]_
    and trains it via stochastic gradient descent. The model is able to train on implicit data
    using negative sample generation, see [X.He2016]_ and the **negative_rate** parameter.

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

        model = rs.FactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
        ))

        updater = rs.FactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=0.05,
            regularization_rate=0.0
        ))
        updater.set_model(model)

        learner = rs.ImplicitGradientLearner()
        learner.add_gradient_updater(updater)
        learner.set_model(model)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=0.0,
            initialize_all=False,
            seed=67439852,
            filter_repeats=False,
        ))
        learner.set_negative_sample_generator(negative_sample_generator)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)
        learner.set_gradient_computer(gradient_computer)

        return (model, learner, [], [])
