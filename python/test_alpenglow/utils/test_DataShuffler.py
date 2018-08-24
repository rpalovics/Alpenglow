import alpenglow as prs
import alpenglow.Getter as rs
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import math
import unittest
from alpenglow.utils import DataShuffler


class TestDataShuffler(unittest.TestCase):
    def test__full_shuffle(self):
        c = DataShuffler(
            input_file="python/test_alpenglow/test_data_4",
            output_file="python/test_alpenglow/tmp/datashuffler_output_file_1",
            shuffle_mode="complete"
        )
        c.run()
    def test__same_timestamp(self):
        c = DataShuffler(
            input_file="python/test_alpenglow/test_data_4",
            output_file="python/test_alpenglow/tmp/datashuffler_output_file_2",
            shuffle_mode="same_timestamp"
        )
        c.run()

