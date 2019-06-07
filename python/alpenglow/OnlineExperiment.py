from .Getter import Getter as rs
from .utils.DataframeData import DataframeData
from .ParameterDefaults import ParameterDefaults
import pandas as pd
import sip


class OnlineExperiment(ParameterDefaults):
    """OnlineExperiment(seed=254938879,top_k=100)
    This is the base class of every online experiment in Alpenglow. It builds
    the general experimental setup needed to run the online training and evaluation
    of a model. It also handles default parameters and the ability to override them
    when instantiating an experiment.

    Subclasses should implement the :code:`config()` method; for more information,
    check the documentation of this method as well.

    Online evaluation in Alpenglow is done by processing the data row-by-row
    and evaluating the model on each new record before providing the model with the
    new information.

    .. image:: /resources/online.png

    Evaluation is done by ranking the next item on the user's toplist and saving the
    rank. If the item is not found in the top :code:`top_k` items, the evaluation step
    returns :code:`NaN`.

    For a brief tutorial on using this class, see :doc:`/getting_started/3-five_minute_tutorial`.

    Parameters
    ----------
    seed : int
        The seed to initialize RNG-s. Should not be 0.
    top_k : int
        The length of the toplists.
    """

    def __init__(self, **parameters):
        super().__init__(**parameters)
        self.used_parameters = set(['seed', 'top_k'])
        if("seed" not in self.parameters):
            self.parameters["seed"] = 254938879
        if("top_k" not in self.parameters):
            self.parameters["top_k"] = 100

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
        rs.collect()
        self.verbose = verbose
        evaluation_start_time = 0 #TODO: start eval at this time

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
            recommender_data_iterator = rs.ShuffleIterator(seed=self.parameters["seed"])
        recommender_data_iterator.set_recommender_data(recommender_data)
        # string attribute_container_name = getPot("set_attribute_container", "");
        # if(attribute_container_name.length()==0) cerr << "WARNING: no attribute container was set into RecommenderData." << endl;
        # else {
        #   InlineAttributeReader* attribute_container = jinja.get<InlineAttributeReader>(attribute_container_name);
        #   recommender_data->set_attribute_container(attribute_container);
        # }
        # data reading finished

        #create experiment
        top_k = self.parameters['top_k']
        seed = self.parameters['seed']


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

        #get components
        (model, learner, filters, loggers) = self._config(top_k, seed)

        #set loggers
        for l in loggers:
            online_experiment.add_logger(l)

        interrupt_logger = rs.InterruptLogger()
        online_experiment.add_logger(interrupt_logger)

        if(verbose):
            proceeding_logger = rs.ProceedingLogger()
            proceeding_logger.set_data_iterator(recommender_data_iterator)
            online_experiment.add_logger(proceeding_logger)

        ranking_logger = self._get_ranking_logger(top_k, evaluation_start_time, self.parameter_default('out_file', out_file), memory_log)
        ranking_logger.set_model(model)
        for f in filters:
            ranking_logger.set_model_filter(f)  # FIXME rank_computer treats only ONE filter
        online_experiment.add_logger(ranking_logger)

        if type(calculate_toplists) is not bool or calculate_toplists:
            print('logging predictions') if self.verbose else None
            model_filter = None
            if len(filters) != 0:
                model_filter = filters[0]
                if(len(filters) > 1):
                    print("Warning: predictionCreator accepts only one model_filter")
            else:
                dummy_model_filter = rs.DummyModelFilter()
                model_filter = dummy_model_filter

            pred_creator = rs.ToplistCreatorPersonalized(
                top_k=top_k,
                exclude_known=exclude_known
            )
            pred_creator.set_filter(model_filter)

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
        self.check_unused_parameters()

        print("running experiment...") if self.verbose else None
        online_experiment.run()
        results = self._finished()
        return results

    def get_predictions(self):
        """If the :code:`calculate_toplists` parameter is set when calling :code:`run`,
        this method can used to acquire the generated toplists.

        Returns
        -------
        pandas.DataFrame
            DataFrame containing the columns record_id, time, user, item, rank and prediction. 

            - **record_id** is the index of the record begin evaluated in the input DataFrame. Generally, there are :code:`top_k` rows with the same record_id.
            - **time** is the time of the evaluation
            - **user** is the user the toplist is generated for
            - **item** is the item of the toplist at the **rank** place
            - **prediction** is the prediction given by the model for the (user, item) pair at the time of evaluation.

        """
        if self.predictions is not None:
            preds = self.predictions.get_predictions()
            preds_df = pd.DataFrame({
                'record_id': preds.ids,
                'time': preds.times,
                'user': preds.users,
                'item': preds.items,
                'rank': preds.ranks,
                'prediction': preds.scores,
            }).sort_values(['record_id'])[['record_id', 'time', 'user', 'item', 'rank', 'prediction']]
            return preds_df
        else:
            return None

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

    def _config(self, top_k, seed):
        """ This method needs to be implemented in every subclass of this class. It is
        called during the :code:`run()` method, and is required to build the model from
        the available C++ components. The expected return type is a python dictionary,
        with at least the :code:`learner`, :code:`model` and :code:`config` keys. The
        config key is expected to further contain at least the :code:`top_k` parameter.
        """
        pass
