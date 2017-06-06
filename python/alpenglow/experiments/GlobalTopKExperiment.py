import alpenglow.Getter as rs
import alpenglow as prs


class GlobalTopKExperiment(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameter_defaults(
            top_k=100,
            min_time=0,
            seed=0,
            out_file=None,
            filters=[],
            loggers=[],
        )
        
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

        pointWise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise(pointWise)
        gradient_computer.set_model(model)
        learner.set_gradient_computer(gradient_computer)

        fmfilter = rs.FactorModelFilter()
        fmfilter.set_model(model)
        fmfilter.set_users(elems['users'])
        fmfilter.set_items(elems['items'])

        prediction_creator = rs.PredictionCreatorGlobal(**self.parameter_defaults(
            top_k=10000,
            # initial_threshold=1000,
            lookback=0
        ))
        prediction_creator.set_model(model)
        prediction_creator.set_filter(fmfilter)
        online_predictor = rs.OnlinePredictor(**self.parameter_defaults(
            min_time=0,
            time_frame=86400,
            file_name=""
        ))
        online_predictor.set_prediction_creator(prediction_creator)

        config['loggers'].append(online_predictor)

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
