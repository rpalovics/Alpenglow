import alpenglow as prs
import alpenglow.Getter as rs
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import math
import unittest
from alpenglow.utils import SameShuffle


class TestSameShuffle(unittest.TestCase):
    def test__get_configurations(self):
        c = SameShuffle(input_file="python/test_alpenglow/test_data_4", output_file="random_output_file_1")
        c.run()

