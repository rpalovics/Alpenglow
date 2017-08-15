from .Getter import Getter as rs
from .DataframeData import DataframeData
from .ParameterDefaults import ParameterDefaults
import pandas as pd
import sip


class OnlineExperiment(ParameterDefaults):
    """This is the base class of every online experiment in Alpenglow. It builds
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

    """

    def __init__(self, **parameters):
        super().__init__(**parameters)
        self.used_parameters = set(['seed'])
        if("seed" not in self.parameters):
            self.parameters["seed"] = 254938879

    def run(self, data, experimentType=None, columns={}, verbose=True, out_file=None, lookback=False, initialize_all=False, max_item=-1, max_user=-1, calculate_toplists=False):
        """
        Parameters
        ----------
        data : pandas.DataFrame or str
            The input data, see :doc:`/getting_started/3-five_minute_tutorial`. If this parameter is a string, it has to be in the format specified by :code:`experimentType`.
        experimentType : str
            The format of the input file if :code:`data` is a string
        columns: dict
            Optionally the mapping of the input DataFrame's columns' names to the expected ones.
        verbose: bool
            Whether to write information about the experiment while running
        out_file: str
            If set, the results of the experiment are also written to the file located at :code:`out_file`.
        lookback: bool
            If set to True, a user's previosly seen items are excluded from the toplist evaluation. The :code:`eval` columns of the input data should be set accordingly.
        calculate_toplists: bool or list
            Whether to actually compute the toplists or just the ranks (the latter is faster). It can be specified on a record-by-record basis, by giving a list of booleans as parameter. The calculated toplists can be acquired after the experiment's end by using :code:`get_predictions`.



        Returns
        -------
        bool
          Description of return value

        """
        rs.collect()
        self.verbose = verbose
        min_time = 0
        max_time = 0

        print("reading data...") if self.verbose else None

        if not isinstance(data, str):
            recommender_data = DataframeData(data, columns=columns)
        else:
            recommender_data = rs.RecommenderData(
                file_name=data,
                type=experimentType
            )
            recommender_data.set_max_time(max_time)
        # TODO set max_item, max_user here
        recommender_data_iterator = rs.ShuffleIterator(seed=self.parameters["seed"])
        recommender_data_iterator.set_recommender_data(recommender_data)

        print("data reading finished") if self.verbose else None

        elems = {}
        configdict = self.config(elems)
        config = configdict['config']
        self.learner = configdict['learner']
        self.model = configdict['model']

        top_k = config['top_k']
        if 'min_time' in config:
            min_time = config['min_time']
        if 'lookback' in config:
            lookback = config['lookback']
        if 'initialize_all' in config:
            initialize_all = config['initialize_all']
        seed = self.parameters["seed"]

        model = self.model
        learner = self.learner

        rank_computer = rs.RankComputer(top_k=top_k, random_seed=43211234)
        rank_computer.set_model(model)

        if 'filters' in config:
            filters = config['filters']
            for f in filters:
                rank_computer.set_model_filter(f)  # FIXME rank_computer treats only ONE filter

        online_experiment = rs.OnlineExperiment(
            random_seed=seed,
            min_time=min_time,
            max_time=max_time,
            top_k=top_k,
            lookback=lookback,
            initialize_all=initialize_all,
            max_item=max_item,
            max_user=max_user
        )

        if type(learner) == list:
          for obj in learner:
            online_experiment.add_updater(obj)
        else:
          online_experiment.add_updater(learner)
        online_experiment.set_recommender_data_iterator(recommender_data_iterator)

        # string attribute_container_name = getPot("set_attribute_container", "");
        # if(attribute_container_name.length()==0) cerr << "WARNING: no attribute container was set into RecommenderData." << endl;
        # else {
        #   InlineAttributeReader* attribute_container = jinja.get<InlineAttributeReader>(attribute_container_name);
        #   recommender_data->set_attribute_container(attribute_container);
        # }

        if 'loggers' in config:
            loggers = config['loggers']
            for l in loggers:
                online_experiment.add_logger(l)

        if type(calculate_toplists) is not bool or calculate_toplists:
            print('logging predictions') if self.verbose else None
            model_filter = None
            if 'filters' in config and len(config['filters']) != 0:
                model_filter = config['filters'][0]
                if(len(config['filters']) > 1):
                    print("Warning: predictionCreator accepts only one model_filter")
            else:
                dummy_model_filter = rs.DummyModelFilter()
                model_filter = dummy_model_filter

            pred_creator = rs.PredictionCreatorPersonalized(
                top_k=top_k,
                lookback=lookback
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

        interrupt_logger = rs.InterruptLogger()
        online_experiment.add_logger(interrupt_logger)

        if(verbose):
            proceeding_logger = rs.ProceedingLogger()
            proceeding_logger.set_data_iterator(recommender_data_iterator)
            online_experiment.add_logger(proceeding_logger)

        ranking_logger = self._get_ranking_logger(top_k, min_time, self.parameter_default('out_file', out_file))
        ranking_logger.set_model(model)
        ranking_logger.set_rank_computer(rank_computer)

        online_experiment.add_logger(ranking_logger)

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
            }).sort_values(['time', 'user','rank'])[['record_id', 'time', 'user', 'item', 'rank', 'prediction']]
            return preds_df
        else:
            return None

    def _get_ranking_logger(self, top_k, min_time, out_file):
        if out_file is None:
            out_file = ""
        else:
            print("logging to file " + out_file) if self.verbose else None
        self.ranking_logs = rs.RankingLogs()
        self.ranking_logs.top_k = top_k
        self.ranking_logger = rs.MemoryRankingLogger(min_time=min_time, out_file=out_file)
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

    def config(elems):
        """ This method needs to be implemented in every subclass of this class. It is
        called during the :code:`run()` method, and is required to build the model from
        the available C++ components. The expected return type is a python dictionary,
        with at least the :code:`learner`, :code:`model` and :code:`config` keys. The
        config key is expected to further contain at least the :code:`top_k` parameter.
        """
        pass
