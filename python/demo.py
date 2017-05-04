import alpenglow as ag
import alpenglow.experiments
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

data = pd.read_csv(
    "/mnt/idms/recSys/data/30M_lastfm/recoded_online_id_artist_first_filtered",
    sep=' ',
    header=None,
    names=['time', 'user', 'item', 'id', 'score', 'eval'],
    nrows=1000000
)

popularityModelExperiment = alpenglow.experiments.PopularityModelExperiment(topK=100, seed=254938879)
popRankings = popularityModelExperiment.run(data, verbose=True)
popResults = ag.NdcgScore(popRankings)
popResults.timeFrame(60 * 60 * 24).plot()
plt.savefig("popularity.png")

factorModelExperiment = alpenglow.experiments.FactorModelExperiment(
    topK=100,
    seed=254938879,
    dimension=10,
    learningRate=0.14,
    negativeRate=100
)
facRankings = factorModelExperiment.run(data, verbose=True)
facResults = ag.NdcgScore(facRankings)
facResults.timeFrame(60 * 60 * 24).plot()
plt.savefig("factor.png")


pd.concat([
    popResults.timeFrame(60 * 60 * 24).rename(columns={'ndcg': 'popularity'}),
    facResults.timeFrame(60 * 60 * 24).rename(columns={'ndcg': 'factor'})
], axis=1).plot()
plt.savefig("popvsfac.png")

param = ag.ThreadedParameterSearch(factorModelExperiment, ag.NdcgScore, threads=5)
param.setParameterValues("learningRate", [0.09, 0.11, 0.14, 0.17, 0.2])
results = param.run(data, verbose=True)
results.set_index('learningRate').plot()
plt.savefig("factor_lr.png")
