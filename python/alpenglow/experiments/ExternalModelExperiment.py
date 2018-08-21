import alpenglow
import alpenglow.Getter as rs

class ExternalModelExperiment(alpenglow.OnlineExperiment):
    """ExternalModelExperiment(period_length=86400,timeframe_length=0,period_mode="time")

    Parameters
    ----------
    period_length : int
        The period length in seconds (or samples, see period_mode).
    timeframe_length : int
        The size of historic time interval to iterate over at every batch model retrain. Leave at the default 0 to retrain on everything.
    period_mode : string
        Either "time" or "samplenum", the unit of period_length and timeframe_length.
    """
    def _config(self, top_k, seed):
        model = rs.ExternalModel(**self.parameter_defaults(
            mode="write",
        ))
        offline_learner = rs.OfflineExternalModelLearner(**self.parameter_defaults(
            out_name_base="batch",
            in_name_base="",
            mode="write",
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
