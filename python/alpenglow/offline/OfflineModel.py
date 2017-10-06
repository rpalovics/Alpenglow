from alpenglow import DataframeData
from alpenglow import Getter as rs
from alpenglow import ParameterDefaults
import numpy as np
import pandas as pd


class OfflineModel(ParameterDefaults):
    """OfflineModel is the base class for all traditional, scikit-learn style models in
    Alpenglow. Example usage:

    .. code-block:: python

        data = pd.read_csv('data')
        train_data = data[data.time < (data.time.min()+250*86400)]
        test_data = data[ (data.time >= (data.time.min()+250*86400)) & (data.time < (data.time.min()+300*86400))]

        exp = ag.offline.models.FactorModel(
            learning_rate=0.07,
            negative_rate=70,
            number_of_iterations=9,
        )
        exp.fit(data)
        test_users = list(set(test_data.user)&set(train_data.user))
        recommendations = exp.recommend(users=test_users)
    """
    def __init__(self, **parameters):
        super().__init__(**parameters)
        self.used_parameters = set(['seed'])
        if("seed" not in self.parameters):
            self.parameters["seed"] = 254938879

    def fit(self, X, y=None, columns={}):
        """Fit the model to a dataset.

        Parameters
        ----------
        X : pandas.DataFrame
            The input data, must contain the columns **user** and **item**. May contain the **score** column as well.
        y : pandas.Series or list
            The target values. If not set (and X doesn't contain the score column), it is assumed to be constant 1 (implicit recommendation).
        columns : dict
            Optionally the mapping of the input DataFrame's columns' names to the expected ones.

        """
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

        matrix = recommender_data.get_full_matrix()
        users = rs.VectorInt([])
        items = rs.VectorInt([])
        recommender_data.get_users_into(users)
        recommender_data.get_items_into(items)

        (model, learner) = self._fit(recommender_data, users, items, matrix)

        created_objects = rs.get_and_clean()
        rs.initialize_all(created_objects)
        for i in created_objects:
            rs.run_self_test(i)
        self.check_unused_parameters()

        learner.fit(recommender_data)

        self.objects = created_objects
        self.model = model
        self.items = items
        self.users = users
        self.matrix = matrix
        self.recommender_data = recommender_data

    def _fit(self, recommender_data, users, items, matrix):
        pass

    def predict(self, X):
        """Predict the target values on X.

        Parameters
        ----------
        X : pandas.DataFrame
            The input data, must contain the columns **user** and **item**.

        Returns
        -------
        list
          List of predictions

        """
        predictor = rs.MassPredictor()
        predictor.set_model(self.model)
        return predictor.predict(X['user'].tolist(), X['item'].tolist())

    def recommend(self, users=None, k=100, exclude_known=True):
        """Give toplist recommendations for users.

        Parameters
        ----------
        users : list
            List of users to give recommendation for.
        k : int
            Size of toplists
        exclude_known : bool
            Whether to exclude (user,item) pairs in the train dataset from the toplists.

        Returns
        -------
        pandas.DataFrame
          DataFrame of recommendations, with columns **user**, **item** and **rank**.

        """

        rs.collect()
        dummy_model_filter = rs.DummyModelFilter()
        dummy_model_filter.set_items(self.items)
        dummy_model_filter.set_users(self.users)

        pred_creator = rs.PredictionCreatorPersonalized(
            top_k=k,
            exclude_known=1 if exclude_known else 0
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
