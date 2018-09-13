import alpenglow as prs
import alpenglow.Getter as rs
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import math
import unittest
import os
from alpenglow.utils import DataShuffler


class TestDataShuffler(unittest.TestCase):
    def clean(self):
       folder = "python/test_alpenglow/tmp/"
       for file_name in os.listdir(folder):
           file_path = os.path.join(folder, file_name)
           if file_name != "empty" and file_name != ".gitignore" and os.path.isfile(file_path):
              os.unlink(file_path) 

    def test__full_shuffle(self):
        self.clean()
        c = DataShuffler(
            input_file="python/test_alpenglow/test_data_5",
            output_file="python/test_alpenglow/tmp/datashuffler_output_file_1",
            shuffle_mode="complete",
            data_format="online_id_noeval",
            seed=17463232
        )
        c.run()
        with open("python/test_alpenglow/test_data_5") as input_file:
            input_file_lines=input_file.readlines()
            with open("python/test_alpenglow/tmp/datashuffler_output_file_1") as output_file:
                output_file_lines=output_file.readlines()
                assert input_file_lines != output_file_lines
                assert len(input_file_lines) == len(output_file_lines)
                output_file_lines_sorted = sorted(output_file_lines, key=lambda x : int(x.split()[3]))
                assert input_file_lines == output_file_lines_sorted
        self.clean()
    def test__same_timestamp(self):
        self.clean()
        c = DataShuffler(
            input_file="python/test_alpenglow/test_data_5",
            output_file="python/test_alpenglow/tmp/datashuffler_output_file_2",
            shuffle_mode="same_timestamp",
            data_format="online_id_noeval",
            seed=17463232
        )
        c.run()
        with open("python/test_alpenglow/test_data_5") as input_file:
            input_file_lines=input_file.readlines()
            with open("python/test_alpenglow/tmp/datashuffler_output_file_2") as output_file:
                output_file_lines=output_file.readlines()
                assert input_file_lines != output_file_lines
                assert len(input_file_lines) == len(output_file_lines)
                output_file_lines_sorted = sorted(output_file_lines, key=lambda x : int(x.split()[3]))
                assert input_file_lines == output_file_lines_sorted
                input_timestamps  = list(map(lambda x : int(x.split()[0]), input_file_lines))
                output_timestamps = list(map(lambda x : int(x.split()[0]),output_file_lines))
                assert input_timestamps == output_timestamps
        self.clean()
    def test__full_shuffle_keep_timestamps(self):
        self.clean()
        c = DataShuffler(
            input_file="python/test_alpenglow/test_data_5",
            output_file="python/test_alpenglow/tmp/datashuffler_output_file_3",
            shuffle_mode="complete_online",
            data_format="online_id_noeval",
            seed=17463232
        )
        c.run()
        with open("python/test_alpenglow/test_data_5") as input_file:
            input_file_lines=input_file.readlines()
            with open("python/test_alpenglow/tmp/datashuffler_output_file_3") as output_file:
                output_file_lines=output_file.readlines()
                assert input_file_lines != output_file_lines
                assert len(input_file_lines) == len(output_file_lines)
                output_file_lines_sorted = sorted(output_file_lines, key=lambda x : int(x.split()[3]))
                assert input_file_lines != output_file_lines_sorted
                input_timestamps  = list(map(lambda x : int(x.split()[0]), input_file_lines))
                output_timestamps = list(map(lambda x : int(x.split()[0]),output_file_lines))
                assert input_timestamps == output_timestamps
        self.clean()

