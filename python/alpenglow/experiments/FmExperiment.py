import alpenglow.Getter as rs
import alpenglow as prs


class FmExperiment(prs.OnlineExperiment):
    """FmExperiment(dimension=10,begin_min=-0.01,begin_max=0.01,learning_rate=0.05,negative_rate=0.0,user_attributes=None,item_attributes=None)

    This class implements an online version of the factorization machine algorithm [Rendle2012]_
    and trains it via stochastic gradient descent. The model is able to train on implicit data
    using negative sample generation, see [X.He2016]_ and the **negative_rate** parameter. Note that
    interactions between separate attributes of a user and between separate attributes of an item are
    not modeled.

    The item and user attributes can be provided through the **user_attributes** and **item_attributes**
    parameters. These each expect a file path pointing to the attribute files. The required format is similar
    to the one used by libfm: the i. line describes the attributes of user **i** in a space sepaterated list
    of **index:value** pairs. For example the line "3:1 10:0.5" as the first line of the file indicates
    that user 0 has 1 as the value of attribute 3, and 0.5 as the value of attribute 10. If the files are
    omitted, an identity matrix is assumed.

    **Notice:** once an attribute file is provided,
    the identity matrix is no longer assumed. If you wish to have a separate latent vector for each id, you
    must explicitly provide the identity matrix in the attribute file itself.

    .. [Rendle2012] Rendle, Steffen. "Factorization machines with libfm." ACM Transactions on Intelligent Systems and Technology (TIST) 3.3 (2012): 57.

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
    user_attributes : string
        The file containing the user attributes, in the format described in the model description. Set None for no attributes (identity matrix).
    item_attributes : string
        The file containing the item attributes, in the format described in the model description. Set None for no attributes (identity matrix).
    """

    def _config(self, top_k, seed):
        model_parameters = self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            user_attributes=None,
            item_attributes=None,
        )
        if(model_parameters['user_attributes'] is not None):
            container = rs.FileSparseAttributeContainer()
            container.load_from_file(model_parameters['user_attributes'])
            model_parameters['user_attributes'] = container

        if(model_parameters['item_attributes'] is not None):
            container = rs.FileSparseAttributeContainer()
            container.load_from_file(model_parameters['item_attributes'])
            model_parameters['item_attributes'] = container

        model = rs.FmModel(**model_parameters)

        updater = rs.FmModelUpdater(**self.parameter_defaults(
            learning_rate=0.05,
        ))
        updater.set_model(model)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=0.0,
            initialize_all=False,
            seed=67439852,
            filter_repeats=False,
        ))
        negative_sample_generator.add_updater(updater)

        return (model, [negative_sample_generator], [], [])
