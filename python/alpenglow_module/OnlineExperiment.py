from .Getter import Getter as rs
from .DataframeIterator import DataframeIterator
from .ParameterSearch import DependentParameter
import pandas as pd
import sip


class OnlineExperiment:
    def __init__(self, **parameters):
        self.parameters = parameters
        self.givenParameters = set(self.parameters.keys())
        self.usedParameters = set(['seed'])
        if("seed" not in self.parameters):
            self.parameters["seed"] = 254938879

    def checkUnusedParameters(self):
        unused = self.givenParameters - self.usedParameters
        if(len(unused) != 0):
            raise TypeError("Unused parameters: "+", ".join(unused))

    def setParameter(self, name, value):
        self.parameters[name] = value
        self.givenParameters |= set([name])

    def parameter_defaults(self, **defaults):
        for k in defaults:
            defaults[k] = self.parameterDefault(k, defaults[k])
        return defaults

    def parameterDefault(self, name, value):
        self.usedParameters |= set([name])
        if name in self.parameters:
            if isinstance(self.parameters[name], DependentParameter):
                return self.parameters[name].eval(self.parameters)
            else:
                return self.parameters[name]
        else:
            return value

    def run(self, data, experimentType=None, columns={}, verbose=True, min_time=0, max_time=0, out_file=None):
        rs.collect()
        self.verbose = verbose

        print("reading data...") if self.verbose else None

        if not isinstance(data, str):
            recommender_data_iterator = DataframeIterator(data, columns=columns)
        else:
            recommenderData = rs.RecommenderData(
                file_name=data,
                type=experimentType
            )
            recommenderData.setMaxTime(max_time)
            recommender_data_iterator = rs.ShuffleIterator(seed=self.parameters["seed"])
            recommender_data_iterator.set_recommender_data(recommenderData)
            recommenderData.init()

        recommender_data_iterator.init()
        print("data reading finished") if self.verbose else None

        train_matrix = rs.SpMatrix()
        users = rs.VectorInt([])
        items = rs.VectorInt([])

        elems = {
            'recommender_data_iterator': recommender_data_iterator,
            'train_matrix': train_matrix,
            'users': users,
            'items': items,
        }
        configdict = self.config(elems)
        config = configdict['config']
        self.learner = configdict['learner']
        self.model = configdict['model']

        top_k = config['top_k']
        if 'min_time' in config:
            min_time = config['min_time']
        seed = self.parameters["seed"]

        topPopContainer = rs.TopPopContainer()
        popContainer = rs.PopContainer()
        onlineRecommender = rs.OnlineRecommender()

        model = self.model
        learner = self.learner

        onlineRecommender.set_model(model)
        onlineRecommender.set_learner(learner)

        onlineDataUpdater = rs.OnlineDataUpdater(
            train_matrix=train_matrix,
            items=[],
            users=[],
            pop=topPopContainer,
            popContainer=popContainer
        )
        onlineDataUpdater.set_items(items)
        onlineDataUpdater.set_users(users)

        rank_computer = rs.RankComputer(top_k=top_k, random_seed=43211234)

        rank_computer.set_train_matrix(train_matrix)
        rank_computer.set_recommender(onlineRecommender)
        rank_computer.set_top_pop_container(topPopContainer)

        if 'filters' in config:
            filters = config['filters']
            for f in filters:
                rank_computer.set_model_filter(f)

        online_experiment = rs.OnlineExperiment(seed=seed)

        online_experiment.set_online_recommender(onlineRecommender)
        online_experiment.set_recommender_data_iterator(recommender_data_iterator)
        online_experiment.set_online_data_updater(onlineDataUpdater)

        # string attribute_container_name = getPot("set_attribute_container", "");
        # if(attribute_container_name.length()==0) cerr << "WARNING: no attribute container was set into RecommenderData." << endl;
        # else {
        #   InlineAttributeReader* attribute_container = jinja.get<InlineAttributeReader>(attribute_container_name);
        #   recommenderData->set_attribute_container(attribute_container);
        # }

        if 'loggers' in config:
            loggers = config['loggers']
            for l in loggers:
                online_experiment.add_logger(l)
                l.init()

        interrupt_logger = rs.InterruptLogger()
        online_experiment.add_logger(interrupt_logger)

        ranking_logger = self.get_ranking_logger(top_k, min_time, self.parameterDefault('out_file',out_file))
        ranking_logger.set_recommender(onlineRecommender)
        ranking_logger.set_rank_computer(rank_computer)
        ranking_logger.init()

        online_experiment.add_logger(ranking_logger)

        createdObjects = rs.getAndClean()
        for i in createdObjects:
            rs.runSelfTest(i)

        self.checkUnusedParameters()

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
        return self.ranking_logs

    def config():
        pass
