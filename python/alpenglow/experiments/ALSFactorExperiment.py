import alpenglow
import alpenglow.Getter as rs

class ALSFactorExperiment(alpenglow.OnlineExperiment):
    def _config(self, top_k, seed):
        model = rs.EigenFactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
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
