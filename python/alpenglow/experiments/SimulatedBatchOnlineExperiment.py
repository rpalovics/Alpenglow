import alpenglow.Getter as rs
import alpenglow as prs

class SimulatedBatchOnlineExperiment(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameter_defaults(
            top_k= 100,
            min_time= 0,
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

        batch_learner = rs.OfflineImplicitGradientLearner(**self.parameter_defaults(
            number_of_iterations=3,
            start_time=-1,
            period_length=86400,
            write_model=False,
            read_model=True,
            clear_model=False,
            learn=False,
            base_out_file_name="",
            base_in_file_name=""
        ))
        batch_learner.set_model(model)
        batch_learner.set_recommender_data_iterator(elems['recommender_data_iterator'])
        batch_learner.add_gradient_updater(updater)


        online_learner = rs.ImplicitGradientLearner()
        online_learner.set_train_matrix(elems['train_matrix'])
        online_learner.add_gradient_updater(updater)
        online_learner.set_model(model)

        learner = rs.CombinedDoubleLayerLearner()
        learner.add_learner(batch_learner)
        learner.add_learner(online_learner)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=100,
            initialize_all=False,
        ))
        negative_sample_generator.set_train_matrix(elems['train_matrix'])
        negative_sample_generator.set_items(elems['items'])
        online_learner.set_negative_sample_generator(negative_sample_generator)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)
        online_learner.set_gradient_computer(gradient_computer)
        
        return {
            'config': config,
            'model': model,
            'learner': learner
        }
