import alpenglow.Getter as rs
import alpenglow as prs


class SvdppExperiment(prs.OnlineExperiment):
    """SvdppExperiment(begin_min=-0.01,begin_max=0.01,dimension=10,learning_rate=0.05,negative_rate=20,use_sigmoid=False,norm_type="exponential",gamma=0.8,user_vector_weight=0.5,history_weight=0.5)

    This class implements an online version of the SVD++ model [Koren2008]_
    The model is able to train on implicit data using negative sample generation, see [X.He2016]_ and the **negative_rate** parameter.
    We apply a decay on the user history, the weight of the older items is smaller.

    .. [Koren2008] Y. Koren, “Factorization Meets the Neighborhood: A Multifaceted Collaborative Filtering Model,” Proc. 14th ACM SIGKDD Int’l Conf. Knowledge Discovery and Data Mining, ACM Press, 2008, pp. 426-434.

    Parameters
    ----------
    begin_min : double
        The factors are initialized randomly, sampling each element uniformly from the interval (begin_min, begin_max).
    begin_max : double
        See begin_min.
    dimension : int
        The latent factor dimension of the factormodel.
    learning_rate : double
        The learning rate used in the stochastic gradient descent updates.
    negative_rate : int
        The number of negative samples generated after each update. Useful for implicit recommendation.
    norm_type : string
        Normalization variants.
    gamma : double
        The constant in the decay function.
    user_vector_weight : double
        The user is modeled with a sum of a user vector and a combination of item vectors. The weight of the two part can be set using these parameters.
    history_weight : double
        See user_vector_weight.
    """
    def _config(self, top_k, seed):
        model = rs.SvdppModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            use_sigmoid=False,
            norm_type="exponential",
            gamma=0.8,
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
            negative_rate=20,
            initialize_all=False,
            seed=928357823,
        ))
        negative_sample_generator.add_updater(gradient_computer)

        return (model, [negative_sample_generator, simple_updater], [], [])
