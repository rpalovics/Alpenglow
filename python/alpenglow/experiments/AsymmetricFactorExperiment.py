import alpenglow.Getter as rs
import alpenglow as prs


class AsymmetricFactorExperiment(prs.OnlineExperiment):
    """AsymmetricFactorExperiment(dimension=10,begin_min=-0.01,begin_max=0.01,learning_rate=0.05,regularization_rate=0.0,negative_rate=20,cumulative_item_updates=True,norm_type="exponential",gamma=0.8)
    
    Implements the recommendation model introduced in [Paterek2007]_.

    .. [Paterek2007] Arkadiusz Paterek. „Improving regularized singular value decomposition for collaborative filtering”. In: Proc. KDD Cup Workshop at SIGKDD’07, 13th ACM Int. Conf. on Knowledge Discovery and Data Mining. San Jose, CA, USA, 2007, pp. 39–42.

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
    norm_type : str
        Type of time decay; either "constant", "exponential" or "disabled".
    gamma : double
        Coefficient of time decay in the case of **norm_type** == "exponential".

    """
    def _config(self, top_k, seed):
        model = rs.AsymmetricFactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            use_sigmoid=False,
            norm_type="exponential",
            gamma=0.8
        ))

        gradient_updater = rs.AsymmetricFactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=0.05,
            cumulative_item_updates=False,
        ))
        gradient_updater.set_model(model)
        simple_updater = rs.AsymmetricFactorModelUpdater()
        simple_updater.set_model(model)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)
        gradient_computer.add_gradient_updater(gradient_updater)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=20,
            initialize_all=False,
            seed=928357823,
        ))
        negative_sample_generator.add_updater(gradient_computer)

        return (model, [negative_sample_generator, simple_updater], [], [])
