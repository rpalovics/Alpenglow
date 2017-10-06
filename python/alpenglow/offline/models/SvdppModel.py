import alpenglow.Getter as rs
import alpenglow.offline


class SvdppModel(alpenglow.offline.OfflineModel):
    """SvdppModel(dimension=10,begin_min=-0.01,begin_max=0.01,learning_rate=0.05,negative_rate=0.0,number_of_iterations=20,cumulative_item_updates=false)

    This class implements the SVD++ model [Koren2008]_
    The model is able to train on implicit data using negative sample generation, see [X.He2016]_ and the **negative_rate** parameter.

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
    negative_rate : int
        The number of negative samples generated after each update. Useful for implicit recommendation.
    number_of_iterations : int
        Number of times to iterate over the training data.
    cumulative_item_updates : boolean
        Cumulative item updates make the model faster but less accurate.
    """
    def _fit(self, recommender_data, users, items, matrix):
        model = rs.SvdppModel(**self.parameter_defaults(
            begin_min=self.parameter_default("begin_min", -0.01),
            begin_max=self.parameter_default("begin_max", 0.01),
            dimension=self.parameter_default("dimension", 10),
            use_sigmoid=False,
            norm_type="constant",
            gamma=1,
            user_vector_weight=0.5,
            history_weight=0.5
        ))

        gradient_updater = rs.SvdppModelGradientUpdater(**self.parameter_defaults(
            learning_rate=0.05,
            cumulative_item_updates=False,
        ))
        gradient_updater.set_model(model)
        simple_updater = rs.SvdppModelUpdater()
        simple_updater.set_model(model)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)
        gradient_computer.add_gradient_updater(gradient_updater)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=9
        ))
        negative_sample_generator.set_train_matrix(matrix)
        negative_sample_generator.set_items(items)
        negative_sample_generator.add_updater(gradient_computer)


        learner = rs.OfflineIteratingOnlineLearnerWrapper(**self.parameter_defaults(
            seed=254938879,
            number_of_iterations=20,
            shuffle=True,
        ))
        learner.add_early_updater(simple_updater)
        learner.add_iterate_updater(negative_sample_generator)

        return (model, learner)
