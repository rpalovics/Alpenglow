from alpenglow.Getter import Getter as rs
from .DataframeData import DataframeData
from alpenglow.ParameterDefaults import ParameterDefaults
import pandas as pd
import sip


class DataShuffler(ParameterDefaults):
    """DataShuffler(seed=254938879,shuffle_mode=complete,input_file,output_file)

    This class is for shuffling datasets.

    Parameters
    ----------
    seed : int
        The seed to initialize RNG-s. Should not be 0.
    shuffle_mode : string
        Possible values: complete, same_timestamp.
    input_file : string
        Input file name.
    output_file : string
        Output file name.
    data_format : string
        Input file format. Available values: online, online_id, online_id_noeval, online_attribute, offline, offlineTimestamp, category. See RecommenderData.cpp for details. Default: online_id.
    """

    def __init__(self, **parameters):
        super().__init__(**parameters)
        self.used_parameters = set(['seed', 'shuffle_mode', 'data_format'])
        if("seed" not in self.parameters):
            self.parameters["seed"] = 254938879
        if("shuffle_mode" not in self.parameters):
            self.parameters["shuffle_mode"] = "complete"
        if("data_format" not in self.parameters):
            self.parameters["data_format"] = "online_id"

    def run(self):
        rs.collect()
        self.verbose = True

        # reading data
        recommender_data = rs.LegacyRecommenderData(
            file_name=self.parameters['input_file'],
            type=self.parameters["data_format"],
            max_time=0
        )
        self.used_parameters.add("input_file")
        recommender_data_iterator = None
        if self.parameters['shuffle_mode'] == "same_timestamp" :
            recommender_data_iterator = rs.ShuffleIterator(seed=self.parameters["seed"])
        elif self.parameters['shuffle_mode'] == "complete_online" :
            recommender_data_iterator = rs.RandomOnlineIterator(seed=self.parameters["seed"])
        else:
            recommender_data_iterator = rs.RandomIterator(seed=self.parameters["seed"])
        recommender_data_iterator.set_recommender_data(recommender_data)
        # data reading finished

        data_shuffle_experiment = rs.OnlineExperiment(
            random_seed=self.parameters["seed"],
            min_time=0,
            max_time=0,
            top_k=0,
            exclude_known=False,
            initialize_all=False,
            max_item=0,
            max_user=0
        )
        data_shuffle_experiment.set_recommender_data_iterator(recommender_data_iterator)

        data_logger = rs.InputLogger(**self.parameter_defaults(
            output_file="",
        ))
        data_shuffle_experiment.add_logger(data_logger)

        interrupt_logger = rs.InterruptLogger()
        data_shuffle_experiment.add_logger(interrupt_logger)

        if(self.verbose):
            proceeding_logger = rs.ProceedingLogger()
            proceeding_logger.set_data_iterator(recommender_data_iterator)
            data_shuffle_experiment.add_logger(proceeding_logger)

        created_objects = rs.get_and_clean()
        rs.set_experiment_environment(data_shuffle_experiment, created_objects)
        rs.initialize_all(created_objects)
        for i in created_objects:
            rs.run_self_test(i)
        self.check_unused_parameters()

        print("shuffling dataset...") if self.verbose else None
        data_shuffle_experiment.run()
