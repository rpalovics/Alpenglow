Introduction
============

Welcome to Alpenglow introduction!

Requirements
-------------

Anaconda + python3 environment

Installation
-------------

.. code-block:: bash

	cd Alpenglow
	conda install libgcc
	pip install .

Development
------------
.. role:: bash(code)
   :language: bash


- For faster recompilation, usen :bash:`export CC="ccache cc"`
- To enable compilation on 4 threads for example, use :bash:`echo 4 > .parallel`
- Reinstall modified version using :bash:`pip install --upgrade --force-reinstall --no-deps .`
- To build and use in the current folder, :bash:`use pip install --upgrade --force-reinstall --no-deps -e .` and :bash:`export PYTHONPATH="$(pwd)/python:$PYTHONPATH"`


Example usage
--------------

Sample dataset: http://info.ilab.sztaki.hu/~fbobee/alpenglow/alpenglow_sample_dataset

.. code-block:: python

	import alpenglow as ag
	from alpenglow.experiments import FactorModelExperiment
	import pandas as pd
	import matplotlib
	matplotlib.use('Agg')
	import matplotlib.pyplot as plt

	data = pd.read_csv("/path/to/sample_dataset")

	factor_model_experiment = FactorModelExperiment(
	    top_k=100,
	    seed=254938879,
	    dimension=10,
	    learning_rate=0.14,
	    negative_rate=100
	)
	fac_rankings = factor_model_experiment.run(data, verbose=True)
	fac_results = ag.NdcgScore(fac_rankings)
	fac_results.time_frame(60 * 60 * 24).plot()
	plt.savefig("factor.png")