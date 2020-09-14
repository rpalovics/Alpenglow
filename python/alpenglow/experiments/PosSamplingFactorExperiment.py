import alpenglow.Getter as rs
import alpenglow as prs


class PosSamplingFactorExperiment(prs.OnlineExperiment):
    """PosSamplingFactorExperiment(dimension=10,begin_min=-0.01,begin_max=0.01,base_learning_rate=0.2,base_regularization_rate=0.0,positive_learning_rate=0.05,positive_regularization_rate=0.0,negative_rate=40,positive_rate=3,pool_size=3000)

    This model implements an online, efficient technique that approximates the learning method of :py:class:`alpenglow.experiments.BatchAndOnlineFactorExperiment`, using fewer update steps.  Similarly to the online MF model (:py:class:`alpenglow.experiments.FactorExperiment`), we only use a single iteration for the model in a temporal order. However, for each interaction, we generate not only negative but also positive samples. The positive samples are selected randomly from past interactions, i.e. we allow the model to re-learn the past. We generate `positive_rate` positive samples along with `negative_rate` negative samples, hence for :math:`t` events, we only take `(1+negative_rate+positive_rate)·t` gradient steps.
    
    The samples are not drawn uniformly from the past, but selected randomly from pool `S` with maximum size `pool_size`. This avoids oversampling interactions that happened at the beginning of the data set. More specifically, for each observed new training instance, we
    
    - update the model by positive_rate samples from pool `S`,
    - delete the selected samples from pool `S` if it already reached `pool_size`,
    - and add the new instance `positive_rate` times to the pool.
    
    For more details, see [frigo2017online]_ .

    .. [frigo2017online] Frigó, E., Pálovics, R., Kelen, D., Kocsis, L., & Benczúr, A. (2017). "Online ranking prediction in non-stationary environments." Section 3.5.

    Parameters
    ----------
    dimension : int
        The latent factor dimension of the factormodel.
    begin_min : double
        The factors are initialized randomly, sampling each element uniformly from the interval (begin_min, begin_max).
    begin_max : double
        See begin_min.
    negative_rate : int
        The number of negative samples generated after each update. Useful for implicit recommendation.
    positive_rate : int
        The number of positive samples generated for each update.
    pool_size : int
        The size of pool for generating positive samples.  See the article for details.
    base_learning_rate : double
        The learning rate used in the stochastic gradient descent updates for the original positive sample and the generated negative samples.
    base_regularization_rate : double
        The coefficient for the L2 regularization term.
    positive_learning_rate : double
        The learning rate used in the stochastic gradient descent updates for the generated positive samples.
    positive_regularization_rate : double
        The coefficient for the L2 regularization term.
    """
    def _config(self, top_k, seed):
        model = rs.FactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
        ))

        updater = rs.FactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=self.parameter_default('base_learning_rate', 0.2),
            regularization_rate=self.parameter_default('base_regularization_rate', 0.0),
        ))
        updater.set_model(model)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)
        gradient_computer.add_gradient_updater(updater)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=100,
            initialize_all=False,
            seed=67439852,
            filter_repeats=False,
        ))
        negative_sample_generator.add_updater(gradient_computer)
        
        positive_updater = rs.FactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=self.parameter_default('positive_learning_rate', 0.05),
            regularization_rate=self.parameter_default('positive_regularization_rate', 0.0),
        ))
        positive_updater.set_model(model)

        positive_point_wise = rs.ObjectiveMSE()
        positive_gradient_computer = rs.GradientComputerPointWise()
        positive_gradient_computer.set_objective(positive_point_wise)
        positive_gradient_computer.set_model(model)
        positive_gradient_computer.add_gradient_updater(positive_updater)
        positive_sample_generator = rs.PooledPositiveSampleGenerator(**self.parameter_defaults(
            positive_rate=3,
            pool_size=3000,
        ))
        positive_sample_generator.add_updater(positive_gradient_computer)



        return (model, [negative_sample_generator, positive_sample_generator], [])
