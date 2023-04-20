from .Getter import Getter as rs
from .utils.DataframeData import DataframeData
from .ParameterDefaults import ParameterDefaults
import pandas as pd
import alpenglow.sip as sip
import alpenglow as prs


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

    def run(self,data,**parameters):
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
        online_experiment_component = prs.components.OnlineExperimentComponent(self) #TODO passing self is not nice, but self is ParameterContainer
        #get components
        top_k = self.parameters['top_k']
        seed = self.parameters['seed']
        rs.collect()
        (model, learner, loggers) = self._config(top_k, seed)
        online_experiment_component.set_model(model)
        if type(learner) == list:
          for obj in learner:
            online_experiment_component.add_updater(obj)
        else:
          online_experiment_component.add_updater(learner)
        for l in loggers:
            online_experiment_component.add_logger(l)

        results = online_experiment_component.run(data,**parameters)
        self.predictions = online_experiment_component.predictions
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
