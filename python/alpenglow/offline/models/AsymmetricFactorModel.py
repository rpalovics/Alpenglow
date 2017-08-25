import alpenglow.Getter as rs
import alpenglow.offline

class AsymmetricFactorModel(alpenglow.offline.OfflineModel):
    """AsymmetricFactorModel(dimension=10,begin_min=-0.01,begin_max=0.01,learning_rate=0.05,regularization_rate=0.0,negative_rate=0,number_of_iterations=9)
    
    Implements the recommendation model introduced in [Paterek2007]_.

    Parameters
    ----------
    dimension : int
        The latent factor dimension of the factormodel.
    begin_min : double
        The factors are initialized randomly, sampling each element uniformly from the interval (begin_min, begin_max).
    begin_max : double
        See begin_min.
    learning_rate : double
        The learning rate used in the stochastic gradient descent updates.
    regularization_rate : double
        The coefficient for the L2 regularization term.
    negative_rate : int
        The number of negative samples generated after each update. Useful for implicit recommendation.
    number_of_iterations : int
        Number of times to iterate over the training data.

    """
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
            negative_rate=0
        ))
        negative_sample_generator.set_train_matrix(matrix)
        negative_sample_generator.set_items(items)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)

        learner = rs.OfflineIteratingImplicitLearner(**self.parameter_defaults(
            seed=254938879,
            number_of_iterations=9,
        ))
        learner.set_gradient_computer(gradient_computer)
        learner.set_negative_sample_generator(negative_sample_generator)
        learner.set_model(model)
        learner.set_recommender_data(recommender_data)
        learner.add_gradient_updater(updater)
        learner.add_early_simple_updater(simple_updater)

        return (model, learner)
