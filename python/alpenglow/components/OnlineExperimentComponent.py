from alpenglow.Getter import Getter as rs
import alpenglow as prs
from alpenglow.utils.DataframeData import DataframeData
import pandas as pd

class OnlineExperimentComponent(prs.Component):

    def __init__(self, *posargs, **parameters):
        super().__init__(*posargs, **parameters)
        self.parameter_container.used_parameters = set(['seed', 'top_k'])
        self.model = None
        self.updaters = []
        self.loggers = []

    def _get_catalog(self):
        return {}

    def set_model(self,model):
        self.model = model
    
    def add_updater(self,updater):
        self.updaters.append(updater)

    def add_logger(self,logger):
        self.loggers.append(logger)

    def run(self,
        data,
        experimentType=None,
        columns={},
        verbose=True,
        out_file=None,
        exclude_known=False,
        initialize_all=False,
        calculate_toplists=False,
        experiment_termination_time=0,
        memory_log=True,
        shuffle_same_time=True
        ):
        """
        Parameters
        ----------
        data : pandas.DataFrame or str
            The input data, see :doc:`/getting_started/3-five_minute_tutorial`. If this parameter is a string, it has to be in the format specified by :code:`experimentType`.
        experimentType : str
            The format of the input file if :code:`data` is a string
        columns : dict
            Optionally the mapping of the input DataFrame's columns' names to the expected ones.
        verbose : bool
            Whether to write information about the experiment while running
        out_file : str
            If set, the results of the experiment are also written to the file located at :code:`out_file`.
        exclude_known : bool
            If set to True, a user's previosly seen items are excluded from the toplist evaluation. The :code:`eval` columns of the input data should be set accordingly.
        calculate_toplists: bool or list
            Whether to actually compute the toplists or just the ranks (the latter is faster). It can be specified on a record-by-record basis, by giving a list of booleans as parameter. The calculated toplists can be acquired after the experiment's end by using :code:`get_predictions`. Setting this to non-False implies shuffle_same_time=False
        experiment_termination_time : int
            Stop the experiment at this timestamp.
        memory_log : bool
            Whether to log the results to memory (to be used optionally with out_file)
        shuffle_same_time : bool
            Whether to shuffle records with the same timestamp randomly.

        Returns
        -------
        DataFrame
          Results DataFrame if memory_log=True, empty DataFrame otherwise

        """
        model = self.model
        learner = self.updaters
        loggers = self.loggers 

        self.verbose = verbose
        evaluation_start_time = 0 #TODO: start eval at this time

        rs.collect()
        # reading data
        if not isinstance(data, str):
            recommender_data = DataframeData(data, columns=columns)
        else:
            recommender_data = rs.LegacyRecommenderData(
                file_name=data,
                type=experimentType,
                experiment_termination_time=experiment_termination_time
            )
        recommender_data.initialize() #read in data -> can find max user, max item
        max_user = recommender_data.get_max_user_id()
        max_item = recommender_data.get_max_item_id()
        recommender_data_iterator = None
        if not shuffle_same_time or calculate_toplists is not False:
            recommender_data_iterator = rs.SimpleIterator()
        else:
            recommender_data_iterator = rs.ShuffleIterator(seed=self.parameter_container.parameters["seed"])
        recommender_data_iterator.set_recommender_data(recommender_data)
        # string attribute_container_name = getPot("set_attribute_container", "");
        # if(attribute_container_name.length()==0) cerr << "WARNING: no attribute container was set into RecommenderData." << endl;
        # else {
        #   InlineAttributeReader* attribute_container = jinja.get<InlineAttributeReader>(attribute_container_name);
        #   recommender_data->set_attribute_container(attribute_container);
        # }
        # data reading finished

        #create experiment
        top_k = self.parameter_container.parameters['top_k']
        seed = self.parameter_container.parameters['seed']


        online_experiment = rs.OnlineExperiment(
            random_seed=seed,
            evaluation_start_time=evaluation_start_time,
            experiment_termination_time=experiment_termination_time,
            top_k=top_k,
            exclude_known=exclude_known,
            initialize_all=initialize_all,
            max_item=max_item,
            max_user=max_user
        )

        #set data
        online_experiment.set_recommender_data_iterator(recommender_data_iterator)

        #set loggers
        for l in loggers:
            online_experiment.add_logger(l)

        interrupt_logger = rs.InterruptLogger()
        online_experiment.add_logger(interrupt_logger)

        if(verbose):
            proceeding_logger = rs.ProceedingLogger()
            proceeding_logger.set_data_iterator(recommender_data_iterator)
            online_experiment.add_logger(proceeding_logger)

        ranking_logger = self._get_ranking_logger(top_k, evaluation_start_time, self.parameter_container.parameter_default('out_file', out_file), memory_log)
        ranking_logger.set_model(model)
        online_experiment.add_logger(ranking_logger)

        if type(calculate_toplists) is not bool or calculate_toplists:
            print('logging predictions') if self.verbose else None

            pred_creator = rs.ToplistCreatorPersonalized(
                top_k=top_k,
                exclude_known=exclude_known
            )
            pred_creator.set_model(model)
            pred_logger = rs.PredictionLogger()
            pred_logger.set_prediction_creator(pred_creator)

            if type(calculate_toplists) is bool:
                online_experiment.add_logger(pred_logger)
            else:
                conditional_meta_logger = rs.ListConditionalMetaLogger(
                    should_run_vector=[int(i) for i in calculate_toplists]
                )
                conditional_meta_logger.set_logger(pred_logger)
                online_experiment.add_logger(conditional_meta_logger)
            self.predictions = pred_logger
        else:
            self.predictions = None

        if type(learner) == list:
          for obj in learner:
            online_experiment.add_updater(obj)
        else:
          online_experiment.add_updater(learner)


        #clean, initialize, test
        created_objects = rs.get_and_clean()
        rs.set_experiment_environment(online_experiment, created_objects)
        rs.initialize_all(created_objects)
        for i in created_objects:
            rs.run_self_test(i)
        self.parameter_container.check_unused_parameters()

        print("running experiment...") if self.verbose else None
        online_experiment.run()
        results = self._finished()
        return results

    def _get_ranking_logger(self, top_k, evaluation_start_time, out_file, memory_log):
        if out_file is None:
            out_file = ""
        else:
            print("logging to file " + out_file) if self.verbose else None
        self.ranking_logs = rs.RankingLogs()
        self.ranking_logs.top_k = top_k
        self.ranking_logger = rs.MemoryRankingLogger(
            evaluation_start_time=evaluation_start_time,
            out_file=out_file,
            memory_log=memory_log,
            top_k=top_k,
            random_seed=43211234
        )
        self.ranking_logger.set_ranking_logs(self.ranking_logs)
        return self.ranking_logger

    def _finished(self):
        logs = self.ranking_logs.logs
        top_k = self.ranking_logs.top_k
        df = pd.DataFrame.from_records(
            [(
                l.id,
                l.time,
                l.score,
                l.user,
                l.item,
                l.prediction,
                l.rank + 1 if l.rank < top_k else None
            ) for l in logs],
            columns=["id", "time", "score", "user", "item", "prediction", "rank"]
        ).set_index("id")
        df['rank']=df['rank'].astype(float)
        df.top_k = top_k
        return df
