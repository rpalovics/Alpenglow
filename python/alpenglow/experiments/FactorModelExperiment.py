import alpenglow.Getter as rs
import alpenglow as prs


class FactorModelExperiment(prs.OnlineExperiment):
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

        model = rs.FactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
        ))

        updater = rs.FactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=0.05,
            regularization_rate=0.0
        ))
        updater.set_model(model)

        learner = rs.ImplicitGradientLearner()
        learner.set_train_matrix(elems['train_matrix'])
        learner.add_gradient_updater(updater)
        learner.set_model(model)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=0.0,
            initialize_all=False,
            seed=0,
        ))
        negative_sample_generator.set_train_matrix(elems['train_matrix'])
        negative_sample_generator.set_items(elems['items'])
        learner.set_negative_sample_generator(negative_sample_generator)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise(point_wise)
        gradient_computer.set_model(model)
        learner.set_gradient_computer(gradient_computer)

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
