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
            shuffle_mode="complete",
            seed=17463232
        )
        c.run()
        input_file=open("python/test_alpenglow/test_data_4")
        input_file_lines=input_file.readlines()
        output_file=open("python/test_alpenglow/tmp/datashuffler_output_file_1")
        output_file_lines=output_file.readlines()
        assert input_file_lines != output_file_lines
        assert len(input_file_lines) == len(output_file_lines)
        output_file_lines_sorted = sorted(output_file_lines, key=lambda x : int(x.split()[3]))
        assert input_file_lines == output_file_lines_sorted
    def test__same_timestamp(self):
        c = DataShuffler(
            input_file="python/test_alpenglow/test_data_5",
            output_file="python/test_alpenglow/tmp/datashuffler_output_file_2",
            shuffle_mode="same_timestamp",
            seed=17463232
        )
        c.run()
        input_file=open("python/test_alpenglow/test_data_5")
        input_file_lines=input_file.readlines()
        output_file=open("python/test_alpenglow/tmp/datashuffler_output_file_2")
        output_file_lines=output_file.readlines()
        assert input_file_lines != output_file_lines
        assert len(input_file_lines) == len(output_file_lines)
        output_file_lines_sorted = sorted(output_file_lines, key=lambda x : int(x.split()[3]))
        assert input_file_lines == output_file_lines_sorted
        input_timestamps  = list(map(lambda x : int(x.split()[0]), input_file_lines))
        output_timestamps = list(map(lambda x : int(x.split()[0]),output_file_lines))
        assert input_timestamps == output_timestamps

