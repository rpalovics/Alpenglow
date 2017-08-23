import alpenglow.Getter as rs
import alpenglow.offline


class AsymmetricFactorModel(alpenglow.offline.OfflineModel):
    def _fit(self, recommender_data, users, items, matrix):
        model = rs.AsymmetricFactorModel(
            begin_min=self.parameter_default("begin_min", -0.01),
            begin_max=self.parameter_default("begin_max", 0.01),
            dimension=self.parameter_default("dimension", 10),
            use_sigmoid=False,
            norm_type="disabled",
            gamma=1
        )

        updater = rs.AsymmetricFactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=0.05,
            regularization_rate=0.0
        ))
        updater.set_model(model)
        simple_updater = rs.AsymmetricFactorModelUpdater()
        simple_updater.set_model(model)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=9
        ))
        negative_sample_generator.set_train_matrix(matrix)
        negative_sample_generator.set_items(items)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)

        learner = rs.OfflineIteratingImplicitLearner(**self.parameter_defaults(
            seed=254938879,
            number_of_iterations=20,
        ))
        learner.set_gradient_computer(gradient_computer)
        learner.set_negative_sample_generator(negative_sample_generator)
        learner.set_model(model)
        learner.set_recommender_data(recommender_data)
        learner.add_gradient_updater(updater)
        learner.add_early_simple_updater(simple_updater)

        return (model, learner)
