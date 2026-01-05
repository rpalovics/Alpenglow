import pandas as pd
import sys
from alpenglow.experiments import ExternalModelExperiment
import pathlib

pathlib.Path("./batches").mkdir(parents=True, exist_ok=True)

data = pd.read_csv('http://info.ilab.sztaki.hu/~fbobee/alpenglow/tutorial_dataset.csv', header=None, names=['time', 'user', 'item'])

exp = ExternalModelExperiment(
    period_length=60 * 60 * 24 * 7 * 4,
    out_name_base="batches/batch",
    mode="write"
)
res = exp.run(data)