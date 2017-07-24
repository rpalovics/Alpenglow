from .Getter import Getter as rs
from .DataframeData import DataframeData
from .ParameterDefaults import ParameterDefaults
import pandas as pd
import sip


class OnlineExperiment(ParameterDefaults):
    def __init__(self, **parameters):
        super().__init__(**parameters)
        self.used_parameters = set(['seed'])
        if("seed" not in self.parameters):
            self.parameters["seed"] = 254938879

    def run(self, data, experimentType=None, columns={}, verbose=True, out_file=None, lookback=False, initialize_all=False, max_item=-1, max_user=-1):
        rs.collect()
        self.verbose = verbose
        min_time = 0
        max_time = 0

        print("reading data...") if self.verbose else None

        if not isinstance(data, str):
            if(max_time != 0):
                # TODO
                pass
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

        online_experiment = rs.OnlineExperiment(random_seed=seed, min_time=min_time, max_time=max_time, top_k=top_k, lookback=lookback, initialize_all=initialize_all, max_item=max_item, max_user=max_user)

        if type(learner) == list:
            for obj in learner:
                online_experiment.add_learner(obj)
        else:
            online_experiment.add_learner(learner)
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

        interrupt_logger = rs.InterruptLogger()
        online_experiment.add_logger(interrupt_logger)

        if(verbose):
            proceeding_logger = rs.ProceedingLogger()
            proceeding_logger.set_data_iterator(recommender_data_iterator)
            online_experiment.add_logger(proceeding_logger)

        ranking_logger = self.get_ranking_logger(top_k, min_time, self.parameter_default('out_file', out_file))
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
        results = self.finished()
        return results

    def get_ranking_logger(self, top_k, min_time, out_file):
        if out_file is None:
            out_file = ""
        else:
            print("logging to file " + out_file) if self.verbose else None
        self.ranking_logs = rs.RankingLogs()
        self.ranking_logs.top_k = top_k
        self.ranking_logger = rs.MemoryRankingLogger(min_time=min_time, out_file=out_file)
        self.ranking_logger.set_ranking_logs(self.ranking_logs)
        return self.ranking_logger

    def finished(self):
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
        df.top_k = top_k
        return df

    def config():
        pass
