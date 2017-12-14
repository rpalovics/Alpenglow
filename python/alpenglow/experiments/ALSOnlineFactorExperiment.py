import alpenglow
import alpenglow.Getter as rs

class ALSOnlineFactorExperiment(alpenglow.OnlineExperiment):
    """ALSOnlineFactorExperiment(dimension=10,begin_min=-0.01,begin_max=0.01,number_of_iterations=15,regularization_lambda=1e-3,alpha=40,implicit=1,clear_before_fit=1,period_length=86400)

    Combines ALSFactorExperiment and FactorExperiment by updating 
    the model periodically with ALS and continously with SGD.

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
        The weight coefficient for positive samples in the error formula. See [Hu2008]_.
    implicit : int
        Valued 1 or 0, indicating whether to run iALS or ALS.
    clear_before_fit : int
        Whether to reset the model after each period.
    period_length : int
        The period length in seconds.
    timeframe_length : int
        The size of historic time interval to iterate over at every batch model retrain. Leave at the default 0 to retrain on everything.
    online_learning_rate : double
        The learning rate used in the online stochastic gradient descent updates.
    online_regularization_rate : double
        The coefficient for the L2 regularization term for online update.
    online_negative_rate : int
        The number of negative samples generated after online each update. Useful for implicit recommendation.
    """

    def _config(self, top_k, seed):

        #
        # batch
        #

        # eigen model
        eigen_model = rs.EigenFactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            seed=254938879,
        ))
        model = rs.FactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
        ))

        # learner
        offline_learner = rs.OfflineEigenFactorModelALSLearner(**self.parameter_defaults(
            number_of_iterations=15,
            regularization_lambda=1e-3,
            alpha=40,
            implicit=1,
            clear_before_fit=1,
        ))
        offline_learner.set_model(eigen_model)
        offline_learner.set_copy_to_model(model)
        als_online_learner = rs.PeriodicOfflineLearnerWrapper(**self.parameter_defaults(
            write_model=False,
            read_model=False,
            clear_model=False,
            learn=True,
            base_out_file_name="",
            base_in_file_name="",
        ))
        als_online_learner.set_model(eigen_model)
        als_online_learner.add_offline_learner(offline_learner)
        
        
        data_generator_parameters = self.parameter_defaults(
            timeframe_length=0,
        )
        if(data_generator_parameters['timeframe_length']==0):
            data_generator = rs.CompletePastDataGenerator()
        else:
            data_generator = rs.TimeframeDataGenerator(**data_generator_parameters)
        als_online_learner.set_data_generator(data_generator)
        period_computer = rs.PeriodComputer(**self.parameter_defaults(
            period_length=86400,
            start_time=-1,
            period_mode="time",
        )) 
        als_online_learner.set_period_computer(period_computer)

        #
        # online
        #

        # updater
        online_updater = rs.FactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=self.parameter_default('online_learning_rate', 0.2),
            regularization_rate=0.0
        ))
        online_updater.set_model(model)


        # objective
        point_wise = rs.ObjectiveMSE()
        online_gradient_computer = rs.GradientComputerPointWise()
        online_gradient_computer.set_objective(point_wise)
        online_gradient_computer.set_model(model)
        online_gradient_computer.add_gradient_updater(online_updater)

        # negative sample generator
        online_negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=self.parameter_default('online_negative_rate', 10),
            initialize_all=False,
            seed=67439852,
            filter_repeats=False,
        ))
        online_negative_sample_generator.add_updater(online_gradient_computer)

        return (model, [als_online_learner, online_negative_sample_generator], [], [])
