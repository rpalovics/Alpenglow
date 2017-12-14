import alpenglow
import alpenglow.Getter as rs

class ALSFactorExperiment(alpenglow.OnlineExperiment):
    """ALSFactorExperiment(dimension=10,begin_min=-0.01,begin_max=0.01,number_of_iterations=15,regularization_lambda=1e-3,alpha=40,implicit=1,clear_before_fit=1,period_length=86400)

    This class implements an online version of the well-known matrix factorization recommendation model [Koren2009]_
    and trains it via Alternating Least Squares in a periodic fashion. The model is able to train on explicit data using traditional ALS,
    and on implicit data using the iALS algorithm [Hu2008]_.

    .. [Hu2008] Hu, Yifan, Yehuda Koren, and Chris Volinsky. "Collaborative filtering for implicit feedback datasets." Data Mining, 2008. ICDM'08. Eighth IEEE International Conference on. Ieee, 2008.

    Parameters
    ----------
    dimension : int
        The latent factor dimension of the factormodel.
    begin_min : double
        The factors are initialized randomly, sampling each element uniformly from the interval (begin_min, begin_max).
    begin_max : double
        See begin_min.
    number_of_iterations : int
        The number of ALS iterations to perform in each period.
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
    """
    def _config(self, top_k, seed):
        model = rs.EigenFactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            seed=67439852,
        ))
        offline_learner = rs.OfflineEigenFactorModelALSLearner(**self.parameter_defaults(
            number_of_iterations=15,
            regularization_lambda=1e-3,
            alpha=40,
            implicit=1,
            clear_before_fit=1,
        ))
        offline_learner.set_model(model)

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
