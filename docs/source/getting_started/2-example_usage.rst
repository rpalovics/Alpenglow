Example usage
--------------

.. code-block:: python

	from alpenglow.experiments import FactorExperiment
	from alpenglow.evaluation import DcgScore
	import pandas as pd
	import matplotlib
	matplotlib.use('Agg')
	import matplotlib.pyplot as plt

	data = pd.read_csv("http://info.ilab.sztaki.hu/~fbobee/alpenglow/alpenglow_sample_dataset")

	factor_model_experiment = FactorExperiment(
	    top_k=100,
	    seed=254938879,
	    dimension=10,
	    learning_rate=0.14,
	    negative_rate=100
	)
	fac_rankings = factor_model_experiment.run(data, verbose=True)
	fac_rankings['dcg'] = DcgScore(fac_rankings)
	fac_rankings['dcg'].groupby((fac_rankings['time']-fac_rankings['time'].min())//86400).mean().plot()
	plt.savefig("factor.png")

