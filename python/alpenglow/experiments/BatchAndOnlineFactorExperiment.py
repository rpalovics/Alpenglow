import alpenglow.Getter as rs
import alpenglow as prs


class BatchAndOnlineFactorExperiment(prs.OnlineExperiment):
    """BatchAndOnlineFactorExperiment(dimension=10,begin_min=-0.01,begin_max=0.01,batch_learning_rate=0.05,batch_regularization_rate=0.0,batch_negative_rate=70,online_learning_rate=0.05,online_regularization_rate=0.0,online_negative_rate=100,period_length=86400)

    Combines BatchFactorExperiment and FactorExperiment by updating 
    the model both in batch and continously.

    Parameters
    ----------
    dimension : int
        The latent factor dimension of the factormodel.
    begin_min : double
        The factors are initialized randomly, sampling each element uniformly from the interval (begin_min, begin_max).
    begin_max : double
        See begin_min.
    batch_learning_rate : double
        The learning rate used in the batch stochastic gradient descent updates.
    batch_regularization_rate : double
        The coefficient for the L2 regularization term for batch updates.
    batch_negative_rate : int
        The number of negative samples generated after each batch update. Useful for implicit recommendation.
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

        model = rs.FactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
        ))

        #
        # batch
        #

        # updater
        batch_updater = rs.FactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=self.parameter_default('batch_learning_rate', 0.05),
            regularization_rate=0.0
        ))
        batch_updater.set_model(model)

        # objective
        point_wise = rs.ObjectiveMSE()
        batch_gradient_computer = rs.GradientComputerPointWise()
        batch_gradient_computer.set_objective(point_wise)
        batch_gradient_computer.set_model(model)
        batch_gradient_computer.add_gradient_updater(batch_updater)

        # negative sample generator
        batch_negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=self.parameter_default('batch_negative_rate', 70),
            initialize_all=False,
            seed=67439852,
            filter_repeats=False,
        ))
        batch_negative_sample_generator.add_updater(batch_gradient_computer)

        batch_offline_learner = rs.OfflineIteratingOnlineLearnerWrapper(**self.parameter_defaults(
            seed=254938879,
            number_of_iterations=3,
            shuffle=True,
        ))
        batch_offline_learner.add_iterate_updater(batch_negative_sample_generator)

        batch_online_learner = rs.PeriodicOfflineLearnerWrapper(**self.parameter_defaults(
            write_model=False,
            read_model=False,
            clear_model=False,
            learn=True,
            base_out_file_name="",
            base_in_file_name="",
        ))
        batch_online_learner.set_model(model)
        batch_online_learner.add_offline_learner(batch_offline_learner)
        
        batch_data_generator_parameters = self.parameter_defaults(
            timeframe_length=0,
        )
        if(batch_data_generator_parameters['timeframe_length']==0):
            print("Full experiment")
            batch_data_generator = rs.CompletePastDataGenerator()
        else:
            print("Timeframe experiment")
            batch_data_generator = rs.TimeframeDataGenerator(**batch_data_generator_parameters)
        batch_online_learner.set_data_generator(batch_data_generator)
        batch_period_computer = rs.PeriodComputer(**self.parameter_defaults(
            period_length=86400,
            start_time=-1,
            period_mode="time",
        )) 
        batch_online_learner.set_period_computer(batch_period_computer)

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
            negative_rate=self.parameter_default('online_negative_rate', 100),
            initialize_all=False,
            seed=67439852,
            filter_repeats=False,
        ))
        online_negative_sample_generator.add_updater(online_gradient_computer)

        learner = [batch_online_learner, online_negative_sample_generator]

        return (model, learner, [], [])
