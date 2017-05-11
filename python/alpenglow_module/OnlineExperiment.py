from .Getter import Getter as rs
from .DataframeIterator import DataframeIterator
import pandas as pd
import sip


class OnlineExperiment:
    def __init__(self, **parameters):
        self.parameters = parameters
        if("seed" not in self.parameters):
            self.parameters["seed"] = 254938879

    def setParameter(self, name, value):
        self.parameters[name] = value

    def parameterDefaults(self, **defaults):
        for k in defaults:
            try:
                defaults[k] = self.parameters[k]
            except KeyError as e:
                pass
        return defaults

    def parameterDefault(self, name, value):
        if name in self.parameters:
            return self.parameters[name]
        else:
            return value

    def run(self, data, experimentType=None, columns={}, verbose=True, minTime=0, maxTime=0, outFile=None):
        rs.collect()
        self.verbose = verbose

        print("reading data...") if self.verbose else None

        if not isinstance(data, str):
            randomAccessIterator = DataframeIterator(data, columns=columns)
        else:
            recommenderData = rs.RecommenderData(
                file_name=data,
                type=experimentType
            )
            recommenderData.setMaxTime(maxTime)
            randomAccessIterator = rs.ShuffleIterator(seed=self.parameters["seed"])
            randomAccessIterator.set_recommender_data(recommenderData)
            recommenderData.init()

        randomAccessIterator.init()
        print("data reading finished") if self.verbose else None

        trainMatrix = rs.SpMatrix()
        users = rs.VectorInt([])
        items = rs.VectorInt([])

        elems = {
            'randomAccessIterator': randomAccessIterator,
            'trainMatrix': trainMatrix,
            'users': users,
            'items': items,
        }
        configdict = self.config(elems)
        config = configdict['config']
        self.learner = configdict['learner']
        self.model = configdict['model']

        topK = config['topK']
        if 'minTime' in config:
            minTime = config['minTime']
        seed = self.parameters["seed"]

        topPopContainer = rs.TopPopContainer()
        popContainer = rs.PopContainer()
        onlineRecommender = rs.OnlineRecommender()

        model = self.model
        learner = self.learner

        onlineRecommender.set_model(model)
        onlineRecommender.set_learner(learner)

        onlineDataUpdater = rs.OnlineDataUpdater(
            trainMatrix=trainMatrix,
            items=[],
            users=[],
            pop=topPopContainer,
            popContainer=popContainer
        )
        onlineDataUpdater.set_items(items)
        onlineDataUpdater.set_users(users)

        rank_computer = rs.RankComputer(topK=topK, random_seed=43211234)

        rank_computer.set_train_matrix(trainMatrix)
        rank_computer.set_recommender(onlineRecommender)
        rank_computer.set_top_pop_container(topPopContainer)

        if 'filters' in config:
            filters = config['filters']
            for f in filters:
                rank_computer.set_model_filter(f)

        online_experiment = rs.OnlineExperiment(seed=seed)

        online_experiment.set_online_recommender(onlineRecommender)
        online_experiment.set_recommender_data_iterator(randomAccessIterator)
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

        ranking_logger = self.get_ranking_logger(topK, minTime, self.parameterDefault('outFile',outFile))
        ranking_logger.set_recommender(onlineRecommender)
        ranking_logger.set_rank_computer(rank_computer)
        ranking_logger.init()

        online_experiment.add_logger(ranking_logger)

        createdObjects = rs.getAndClean()
        for i in createdObjects:
            rs.runSelfTest(i)

        print("running experiment...") if self.verbose else None
        online_experiment.run()
        results = self.finished()
        return results

    def get_ranking_logger(self, topK, minTime, outFile):
        if outFile is None:
            outFile = ""
        else:
            print("logging to file " + outFile) if self.verbose else None
        self.ranking_logs = rs.RankingLogs()
        self.ranking_logs.topK = topK
        self.ranking_logger = rs.MemoryRankingLogger(min_time=minTime, out_file=outFile)
        self.ranking_logger.set_ranking_logs(self.ranking_logs)
        return self.ranking_logger

    def finished(self):
        return self.ranking_logs

    def config():
        pass
