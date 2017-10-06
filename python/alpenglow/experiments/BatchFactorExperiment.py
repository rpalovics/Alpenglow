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

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)
        gradient_computer.add_gradient_updater(updater)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=0,
            initialize_all=False,
            seed=67439852,
            filter_repeats=False,
        ))
        negative_sample_generator.add_updater(gradient_computer)

        offline_learner = rs.OfflineIteratingOnlineLearnerWrapper(**self.parameter_defaults(
            seed=254938879,
            number_of_iterations=3,
            shuffle=True,
        ))
        offline_learner.add_iterate_updater(negative_sample_generator)

        online_learner = rs.PeriodicOfflineLearnerWrapper(**self.parameter_defaults(
            write_model=False,
            read_model=False,
            clear_model=False,
            learn=True,
            base_out_file_name="",
            base_in_file_name="",
        ))
        online_learner.set_model(model)
        online_learner.add_offline_learner(offline_learner)
        
        data_generator_parameters = self.parameter_defaults(
            timeframe_length=0,
        )
        if(data_generator_parameters['timeframe_length']==0):
            data_generator = rs.CompletePastDataGenerator()
        else:
            data_generator = rs.TimeframeDataGenerator(**data_generator_parameters)
        online_learner.set_data_generator(data_generator)
        period_computer = rs.PeriodComputer(**self.parameter_defaults(
            period_length=86400,
            start_time=-1,
            period_mode="time",
        )) 
        online_learner.set_period_computer(period_computer)


        return (model, online_learner, [], [])
