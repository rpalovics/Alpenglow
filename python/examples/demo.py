import alpenglow as ag
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

data = pd.read_csv(
    "/mnt/idms/recSys/data/30M_lastfm/recoded_online_id_artist_first_filtered",
    sep=' ',
    header=None,
    names=['time', 'user', 'item', 'id', 'score', 'eval'],
    nrows=100000
)

popularity_model_experiment = alpenglow.experiments.PopularityExperiment(top_k=100, seed=254938879)
pop_rankings = popularity_model_experiment.run(data, verbose=True)
pop_rankings['dcg'] = ag.evaluation.DcgScore(pop_rankings)
day_groups = (pop_rankings['time']-pop_rankings['time'].min())//86400
pop_daily_avg = pop_rankings['dcg'].groupby(day_groups).mean()
plt.figure()
pop_daily_avg.plot()
plt.savefig("popularity.png")

factor_model_experiment = alpenglow.experiments.FactorExperiment(
    top_k=100,
    seed=254938879,
    dimension=10,
    learning_rate=0.14,
    negative_rate=100
)
fac_rankings = factor_model_experiment.run(data, verbose=True)
fac_rankings['dcg'] = ag.evaluation.DcgScore(fac_rankings)
fac_daily_avg = fac_rankings['dcg'].groupby(day_groups).mean()
plt.figure()
fac_daily_avg.plot()
plt.savefig("factor.png")

plt.figure()
pd.concat([
    pop_daily_avg.to_frame().rename(columns={'dcg': 'popularity'}),
    fac_daily_avg.to_frame().rename(columns={'dcg': 'factor'})
], axis=1).plot()
plt.savefig("popvsfac.png")

param = ag.utils.ThreadedParameterSearch(factor_model_experiment, ag.evaluation.DcgScore, threads=5)
param.set_parameter_values("learning_rate", [0.09, 0.11, 0.14, 0.17, 0.2])
results = param.run(data, verbose=True)
results.set_index('learning_rate').plot()
plt.savefig("factor_lr.png")
