import alpenglow.Getter as rs
import alpenglow.offline


class FactorModel(alpenglow.offline.OfflineModel):
    """FactorModel(dimension=10,begin_min=-0.01,begin_max=0.01,learning_rate=0.05,regularization_rate=0.0,negative_rate=0.0,number_of_iterations=9)

    This class implements the well-known matrix factorization recommendation model [Koren2009]_
    and trains it via stochastic gradient descent. The model is able to train on implicit data
    using negative sample generation, see [X.He2016]_ and the **negative_rate** parameter.

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
        model = rs.FactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
            seed=254938879,
        ))

        updater = rs.FactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=0.05,
            regularization_rate=0.0
        ))
        updater.set_model(model)


        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)
        gradient_computer.add_gradient_updater(updater)


        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=0
        ))
        negative_sample_generator.set_train_matrix(matrix)
        negative_sample_generator.set_items(items)
        negative_sample_generator.add_updater(gradient_computer)

        learner = rs.OfflineIteratingOnlineLearnerWrapper(**self.parameter_defaults(
            seed=254938879,
            number_of_iterations=9,
            shuffle=True,
        ))
        learner.add_iterate_updater(negative_sample_generator)

        return (model, learner)
