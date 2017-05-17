import alpenglow.Getter as rs
import alpenglow as prs

class SimulatedBatchExperiment(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameterDefaults(
            top_k= 100,
            min_time= 0
        )

        model = rs.FactorModel(**self.parameterDefaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
        ))

        updater = rs.FactorModelGradientUpdater(**self.parameterDefaults(
            learning_rate=0.05,
            regularization_rate=0.0
        ))
        updater.set_model(model)

        learner = rs.OfflineImplicitGradientLearner(**self.parameterDefaults(
            number_of_iterations=3,
            start_time=-1,
            period_length=86400,
            write_model=False,
            read_model=False,
            clear_model=False,
            learn=True,
            base_out_file_name="",
            base_in_file_name=""
        ))
        learner.set_model(model)
        learner.set_recommender_data_iterator(elems['recommender_data_iterator'])
        learner.add_gradient_updater(updater)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameterDefaults(
            negative_rate=3,
            initialize_all=False,
        ))
        negative_sample_generator.setTrainMatrix(elems['train_matrix'])
        negative_sample_generator.set_items(elems['items'])
        learner.set_negative_sample_generator(negative_sample_generator)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise(point_wise)
        gradient_computer.set_model(model)
        learner.set_gradient_computer(gradient_computer)
        learner.init()

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
