from alpenglow.experiments import FactorExperiment
from alpenglow.evaluation import DcgScore
import pandas as pd

data = pd.read_csv("http://info.ilab.sztaki.hu/~fbobee/alpenglow/alpenglow_sample_dataset")

factor_model_experiment = FactorExperiment(
    top_k=100,
    seed=254938879,
    dimension=10,
    learning_rate=0.14,
    negative_rate=100
)

rankings = factor_model_experiment.run(data, verbose=True)

