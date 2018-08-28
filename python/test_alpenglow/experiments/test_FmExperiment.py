import alpenglow as prs
import alpenglow.Getter as rs
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import math
import numpy as np


class TestFactorExperiment:
    def test_factorExperiment(self):
        fmExperiment = alpenglow.experiments.FmExperiment(
            top_k=100,
            seed=254938879,
            dimension=10,
            learning_rate=0.05,
            negative_rate=10,
            user_attributes="python/test_alpenglow/test_data_4_user_attr",
            item_attributes="python/test_alpenglow/test_data_4_item_attr"
        )
        facRankings = fmExperiment.run(
            "python/test_alpenglow/test_data_4",
            experimentType="online_id",
            verbose=True)
        # print([i for i in facRankings["rank"].fillna(101)])
        assert facRankings.top_k == 100
        desired_ranks = [101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 4.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 4.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 4.0, 101.0, 101.0, 101.0, 1.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 5.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 10.0, 13.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 9.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 10.0, 101.0, 59.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 67.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 62.0, 6.0, 101.0, 101.0, 101.0, 101.0, 15.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 1.0, 101.0, 9.0, 101.0, 101.0, 101.0, 101.0, 101.0, 12.0, 101.0, 101.0, 101.0, 76.0, 101.0, 69.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 7.0, 101.0, 101.0, 101.0, 101.0, 19.0, 101.0, 101.0, 101.0, 101.0, 101.0, 69.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 75.0, 101.0, 101.0, 101.0, 101.0, 17.0, 101.0, 101.0, 101.0, 84.0, 10.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 78.0, 1.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 5.0, 101.0, 101.0, 101.0, 11.0, 101.0, 79.0, 88.0, 101.0, 101.0, 101.0, 85.0, 101.0, 101.0, 101.0, 101.0, 101.0, 87.0, 79.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 27.0, 14.0, 101.0, 101.0, 11.0, 101.0, 101.0, 101.0, 101.0, 13.0, 95.0, 3.0, 101.0, 101.0, 101.0, 101.0, 101.0, 15.0, 101.0, 101.0, 101.0, 101.0, 101.0, 15.0, 101.0, 101.0, 1.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 97.0, 101.0, 101.0, 83.0, 101.0, 4.0, 101.0, 101.0, 21.0, 1.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 12.0, 101.0, 101.0, 4.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 2.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 25.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 98.0, 101.0, 101.0, 11.0, 101.0, 101.0, 16.0, 101.0, 13.0, 101.0, 101.0, 8.0, 4.0, 14.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 1.0, 101.0, 101.0, 5.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 96.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 21.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 20.0, 101.0, 18.0, 101.0, 101.0, 101.0, 23.0, 11.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 34.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 22.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 17.0, 18.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 26.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 17.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 24.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 19.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 32.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 19.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 52.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 16.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 11.0, 101.0, 101.0, 3.0, 101.0, 101.0, 101.0, 7.0, 101.0, 37.0, 41.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 29.0, 101.0, 101.0, 101.0, 101.0, 101.0, 34.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 37.0, 101.0, 34.0, 3.0, 1.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 41.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 44.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 15.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 23.0, 101.0, 101.0, 101.0, 101.0, 21.0, 6.0, 101.0, 101.0, 101.0, 101.0, 101.0, 47.0, 101.0, 101.0, 101.0, 41.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 47.0, 101.0, 101.0, 101.0, 34.0, 22.0, 101.0, 101.0, 49.0, 22.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 49.0, 101.0, 48.0, 33.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 30.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 6.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 20.0, 101.0, 101.0, 101.0, 45.0, 101.0, 10.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 37.0, 101.0, 101.0, 101.0, 101.0, 101.0, 24.0, 5.0, 101.0, 55.0, 101.0, 101.0, 101.0, 14.0, 101.0, 35.0, 101.0, 101.0, 101.0, 58.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 10.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 1.0, 41.0, 6.0, 101.0, 101.0, 101.0, 101.0, 52.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 19.0, 101.0, 101.0, 101.0, 59.0, 101.0, 101.0, 101.0, 71.0, 101.0, 7.0, 66.0, 14.0, 101.0, 31.0, 101.0, 101.0, 101.0, 79.0, 101.0, 101.0, 101.0, 36.0, 101.0, 101.0, 101.0, 101.0, 101.0]
        assert [i for i in facRankings["rank"].fillna(101)] == desired_ranks

    def test_factorExperiment2(self):
        fmExperiment = alpenglow.experiments.FmExperiment(
            top_k=100,
            seed=254938879,
            dimension=10,
            learning_rate=0.05,
            negative_rate=10,
            user_attributes="python/test_alpenglow/test_data_4_user_attr"
        )
        facRankings = fmExperiment.run(
            "python/test_alpenglow/test_data_4",
            experimentType="online_id",
            verbose=True)
        assert facRankings.top_k == 100
        # print([i for i in facRankings["rank"].fillna(101)])
        desired_ranks = [101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 10.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 3.0, 101.0, 101.0, 1.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 26.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 2.0, 101.0, 101.0, 101.0, 11.0, 4.0, 101.0, 101.0, 101.0, 101.0, 101.0, 40.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 1.0, 101.0, 19.0, 27.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 18.0, 101.0, 7.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 6.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 47.0, 101.0, 6.0, 101.0, 101.0, 101.0, 48.0, 101.0, 40.0, 101.0, 101.0, 50.0, 101.0, 101.0, 101.0, 101.0, 20.0, 6.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 62.0, 9.0, 101.0, 101.0, 101.0, 101.0, 45.0, 101.0, 101.0, 63.0, 61.0, 8.0, 2.0, 10.0, 101.0, 8.0, 101.0, 101.0, 101.0, 101.0, 101.0, 50.0, 101.0, 101.0, 101.0, 73.0, 101.0, 54.0, 101.0, 52.0, 101.0, 101.0, 101.0, 101.0, 46.0, 101.0, 101.0, 101.0, 101.0, 79.0, 101.0, 25.0, 76.0, 101.0, 101.0, 47.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 2.0, 85.0, 101.0, 101.0, 101.0, 101.0, 60.0, 83.0, 8.0, 101.0, 101.0, 101.0, 101.0, 17.0, 101.0, 24.0, 101.0, 81.0, 22.0, 101.0, 101.0, 101.0, 101.0, 101.0, 23.0, 74.0, 64.0, 13.0, 3.0, 13.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 96.0, 101.0, 6.0, 101.0, 91.0, 101.0, 101.0, 101.0, 42.0, 101.0, 27.0, 82.0, 101.0, 101.0, 101.0, 80.0, 101.0, 12.0, 101.0, 101.0, 101.0, 64.0, 2.0, 101.0, 101.0, 101.0, 101.0, 40.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 6.0, 101.0, 101.0, 73.0, 101.0, 98.0, 74.0, 101.0, 77.0, 34.0, 101.0, 3.0, 101.0, 101.0, 101.0, 101.0, 64.0, 101.0, 78.0, 101.0, 101.0, 25.0, 78.0, 101.0, 101.0, 7.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 42.0, 7.0, 101.0, 101.0, 20.0, 101.0, 79.0, 13.0, 101.0, 101.0, 67.0, 101.0, 92.0, 69.0, 83.0, 101.0, 101.0, 101.0, 79.0, 101.0, 101.0, 4.0, 101.0, 18.0, 33.0, 101.0, 101.0, 101.0, 101.0, 101.0, 74.0, 6.0, 43.0, 3.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 72.0, 101.0, 10.0, 101.0, 101.0, 101.0, 25.0, 101.0, 32.0, 101.0, 85.0, 101.0, 49.0, 101.0, 101.0, 101.0, 85.0, 14.0, 101.0, 37.0, 101.0, 101.0, 32.0, 101.0, 15.0, 35.0, 101.0, 10.0, 101.0, 101.0, 17.0, 101.0, 5.0, 16.0, 62.0, 23.0, 101.0, 101.0, 64.0, 101.0, 101.0, 12.0, 101.0, 101.0, 37.0, 101.0, 80.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 17.0, 101.0, 74.0, 80.0, 101.0, 101.0, 101.0, 101.0, 68.0, 6.0, 101.0, 101.0, 101.0, 101.0, 34.0, 14.0, 101.0, 101.0, 101.0, 94.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 43.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 49.0, 101.0, 101.0, 62.0, 8.0, 101.0, 89.0, 101.0, 11.0, 101.0, 101.0, 101.0, 1.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 4.0, 101.0, 7.0, 20.0, 101.0, 101.0, 101.0, 86.0, 101.0, 48.0, 101.0, 101.0, 47.0, 101.0, 101.0, 69.0, 101.0, 101.0, 3.0, 101.0, 101.0, 101.0, 71.0, 101.0, 101.0, 101.0, 101.0, 101.0, 57.0, 101.0, 88.0, 101.0, 101.0, 17.0, 101.0, 101.0, 101.0, 10.0, 101.0, 66.0, 101.0, 101.0, 101.0, 101.0, 69.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 79.0, 101.0, 101.0, 60.0, 101.0, 101.0, 101.0, 101.0, 28.0, 59.0, 101.0, 101.0, 101.0, 101.0, 101.0, 5.0, 101.0, 101.0, 101.0, 101.0, 81.0, 101.0, 40.0, 101.0, 101.0, 101.0, 101.0, 78.0, 101.0, 101.0, 101.0, 101.0, 101.0, 8.0, 32.0, 45.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 33.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 34.0, 101.0, 101.0, 19.0, 101.0, 101.0, 101.0, 101.0, 27.0, 101.0, 101.0, 101.0, 101.0, 101.0, 35.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 57.0, 101.0, 7.0, 101.0, 32.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 24.0, 101.0, 101.0, 101.0, 36.0, 42.0, 101.0, 101.0, 101.0, 101.0, 101.0, 8.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 4.0, 101.0, 101.0, 101.0, 81.0, 71.0, 101.0, 101.0, 84.0, 101.0, 101.0, 101.0, 22.0, 101.0, 4.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 87.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 58.0, 44.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 58.0, 101.0, 101.0, 101.0, 101.0, 85.0, 101.0, 101.0, 101.0, 101.0, 15.0, 101.0, 101.0, 101.0, 101.0, 73.0, 101.0, 101.0, 101.0, 44.0, 101.0, 101.0, 101.0, 101.0, 44.0, 101.0, 101.0, 101.0, 14.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 67.0, 94.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 92.0, 62.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 48.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 40.0, 37.0, 101.0, 101.0, 101.0, 101.0, 101.0, 42.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 21.0, 101.0, 53.0, 51.0, 101.0, 96.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 4.0, 13.0, 101.0, 85.0, 101.0, 101.0, 101.0, 5.0, 101.0, 101.0, 55.0, 101.0, 101.0, 101.0, 25.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 73.0, 101.0, 101.0, 36.0, 101.0, 101.0, 74.0, 101.0, 80.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 93.0, 10.0, 101.0, 101.0, 101.0, 92.0, 87.0, 101.0, 78.0, 39.0, 101.0, 101.0, 101.0, 101.0, 48.0, 38.0, 101.0, 101.0, 101.0, 101.0, 43.0, 73.0, 24.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 4.0, 101.0, 101.0, 101.0, 15.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 34.0, 56.0, 91.0, 101.0, 101.0, 101.0, 53.0, 20.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 37.0, 101.0, 101.0, 101.0, 101.0, 88.0, 101.0, 101.0, 101.0, 63.0, 101.0, 65.0, 11.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 99.0, 101.0, 101.0, 91.0, 101.0, 101.0, 101.0, 101.0, 7.0, 1.0, 96.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 71.0, 10.0, 101.0, 101.0, 101.0, 101.0, 101.0, 11.0, 32.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 94.0, 101.0, 12.0, 93.0, 51.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 59.0, 59.0, 101.0, 101.0, 101.0, 71.0, 101.0, 101.0, 101.0, 101.0, 101.0, 58.0, 101.0, 101.0, 101.0, 101.0, 94.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 53.0, 53.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 87.0, 101.0, 21.0, 101.0, 9.0, 101.0, 101.0, 46.0, 101.0, 101.0, 79.0, 6.0, 101.0, 49.0, 101.0, 101.0, 101.0, 101.0, 59.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 29.0, 101.0, 101.0, 101.0, 101.0, 6.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 101.0, 2.0, 101.0, 101.0, 101.0, 101.0, 101.0, 44.0, 53.0, 101.0, 101.0, 86.0, 101.0, 101.0, 101.0]
        assert list(facRankings["rank"].fillna(101)) == desired_ranks