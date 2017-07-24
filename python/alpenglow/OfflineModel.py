from .DataframeData import DataframeData
from .Getter import Getter as rs
from .ParameterDefaults import ParameterDefaults
import numpy as np
import pandas as pd


class OfflineModel(ParameterDefaults):
    def __init__(self, **parameters):
        super().__init__(**parameters)
        self.used_parameters = set(['seed'])
        if("seed" not in self.parameters):
            self.parameters["seed"] = 254938879

    def fit(self, X, y=None, columns={}):
        rs.collect()
        data = X
        if y is None:
            if 'score' not in X:
                data['score'] = np.ones(len(X))
        else:
            if 'score' in X:
                raise ValueError("y and score column both provided")
            else:
                data['score'] = y

        recommender_data = DataframeData(data, columns=columns)

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
        self.items = items
        self.users = users
        self.matrix = matrix
        self.recommender_data = recommender_data

    def predict(self, X):
        predictor = rs.MassPredictor()
        predictor.set_model(self.model)
        return predictor.predict(X['user'].tolist(), X['item'].tolist())

    def recommend(self, users=None, k=100, exclude_known=True):
        rs.collect()
        dummy_model_filter = rs.DummyModelFilter()
        dummy_model_filter.set_items(self.items)
        dummy_model_filter.set_users(self.users)

        pred_creator = rs.PredictionCreatorPersonalized(
            top_k=k,
            lookback=1 if exclude_known else 0
        )

        pred_creator.set_filter(dummy_model_filter)
        pred_creator.set_train_matrix(self.matrix)
        pred_creator.set_model(self.model)

        ranking_computer = rs.OfflineRankingComputer(
            top_k=k
        )
        ranking_computer.set_items(self.items)
        if users is None:
            ranking_computer.set_users(self.users)
        else:
            ranking_computer.set_users(rs.VectorInt(pd.Series(users).unique().tolist()))

        ranking_computer.set_toplist_creator(pred_creator)

        created_objects = rs.get_and_clean()
        # rs.initialize_all(created_objects)
        for i in created_objects:
            rs.run_self_test(i)

        preds = ranking_computer.compute()
        preds_df = pd.DataFrame({
            'user': preds.users,
            'item': preds.items,
            'rank': preds.ranks
        }).sort_values(['user', 'rank'])[['user', 'item', 'rank']]
        return preds_df
