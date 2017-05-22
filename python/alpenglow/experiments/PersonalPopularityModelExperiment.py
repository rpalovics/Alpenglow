import alpenglow.Getter as rs
import alpenglow as prs


class PersonalPopularityModelExperiment(prs.OnlineExperiment):
    def config(self, elems):
        proceeding_logger = rs.ProceedingLogger()
        proceeding_logger.set_data_iterator(elems['recommender_data_iterator'])

        config = self.parameter_defaults(
            top_k=100,
            min_time=0,
            seed=0,
            out_file=None,
            filters=[],
        )
        config['loggers'] = [proceeding_logger] if self.verbose else []

        model = rs.PersonalPopularityModel()
        updater = rs.PersonalPopularityModelUpdater()
        updater.set_model(model)
        learner = rs.SimpleLearner()
        learner.add_simple_updater(updater)
        learner.set_model(model)

        model = model
        learner = learner

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
