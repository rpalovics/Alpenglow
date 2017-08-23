import alpenglow.Getter as rs
import alpenglow as prs


class BatchFactorExperiment(prs.OnlineExperiment):
    """BatchFactorExperiment(dimension=10,begin_min=-0.01,begin_max=0.01,learning_rate=0.05,regularization_rate=0.0,negative_rate=0.0,number_of_iterations=3,period_length=86400,timeframe_length=0,clear_model=False)

    Batch version of :py:class:`alpenglow.experiments.FactorExperiment.FactorExperiment`,
    meaning it retrains its model periodically nd evaluates the latest model between two
    training points in an online fashion.

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
    number_of_iterations : int
        The number of iterations over the data in model retrain.
    period_length : int
        The amount of time between model retrains (seconds).
    timeframe_length : int
        The size of historic time interval to iterate over at every model retrain. Leave at the default 0 to retrain on everything.
    clear_model: bool
        Whether to clear the model between retrains.
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

        learner_parameters = self.parameter_defaults(
            number_of_iterations=3,
            start_time=-1,
            period_length=86400,
            write_model=False,
            read_model=False,
            clear_model=False,
            learn=True,
            base_out_file_name="",
            base_in_file_name="",
            timeframe_length=0,
        )

        if(learner_parameters['timeframe_length']==0):
            learner_parameters.pop('timeframe_length', None)
            learner = rs.OfflineImplicitGradientLearner(**learner_parameters)
        else:
            learner = rs.PeriodicTimeframeImplicitGradientLearner(**learner_parameters)

        learner.set_model(model)
        learner.add_gradient_updater(updater)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=0,
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