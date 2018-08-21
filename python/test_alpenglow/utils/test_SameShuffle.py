import alpenglow as prs
import alpenglow.Getter as rs
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import math
import unittest
from alpenglow.utils import SameShuffle


class TestSameShuffle(unittest.TestCase):
    def test__full_shuffle(self):
        c = SameShuffle(
            input_file="python/test_alpenglow/test_data_4",
            output_file="random_output_file_1",
            shuffle_mode="same_timestamp"
        )
        c.run()
    def test__same_timestamp(self):
        c = SameShuffle(
            input_file="python/test_alpenglow/test_data_4",
            output_file="random_output_file_2",
            shuffle_mode="same_timestamp"
        )
        c.run()

