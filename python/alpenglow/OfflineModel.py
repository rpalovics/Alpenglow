from .DataframeData import DataframeData
from .Getter import Getter as rs
from .ParameterDefaults import ParameterDefaults


class OfflineModel(ParameterDefaults):
    def __init__(self, **parameters):
        super().__init__(**parameters)
        self.used_parameters = set(['seed'])
        if("seed" not in self.parameters):
            self.parameters["seed"] = 254938879

    def fit(self, data, columns={}):
        rs.collect()
        recommender_data = DataframeData(data, columns=columns)
        recommender_data_iterator = rs.ShuffleIterator(seed=self.parameter_default('seed', 254938879))
        recommender_data_iterator.set_recommender_data(recommender_data)

        matrix = recommender_data.matrix()
        users = rs.VectorInt([])
        items = rs.VectorInt([])
        recommender_data.get_users_into(users)
        recommender_data.get_items_into(items)

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

        created_objects = rs.get_and_clean()
        rs.initialize_all(created_objects)
        for i in created_objects:
            rs.run_self_test(i)
        self.check_unused_parameters()

        learner.iterate()

        self.objects = created_objects
        self.model = model

    def predict(self, user_item_pairs):
        predictor = rs.MassPredictor()
        predictor.set_model(self.model)
        return predictor.predict(user_item_pairs['user'].tolist(), user_item_pairs['item'].tolist())
