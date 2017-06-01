Five minute tutorial
====================

In this tutorial we are going to learn the basic concepts of using Alpenglow by evaluation various baseline models on real world data and building a simple combined model.

The data
--------

You can find the dataset [todo]. This is a processed version of the 30M dataset [todo], where we

- only kept users above a certain activity threshold
- only kept the first events of listening sessions
- recoded the items so they represent artists instead of tracks

Let's start by importing the csv using pandas.


.. role:: python(code)
   :language: python

.. code-block:: python

	import pandas as pd
	data = pd.read_csv('artist_data_10_1800')
	print(data.columns)

Output::

	Index(['time', 'user', 'item', 'id', 'score', 'eval', 'category'], dtype='object')

To run online experiments, you will need time-series data of user-item interactions in similar format to the above. The only neccessary columns are the :python:`'user'` and :python:`'item'` columns -- the rest will be autofilled if missing. The most important columns are the following:

- **time**: integer, the timestamp of the record. Controls various things, like evaluation timeframes or batch learning epochs. Defaults to :python:`range(0,len(data))` if missing.
- **user**: integer, the user the activity belongs to. This column is required.
- **item**: integer, the item the activity belongs to. This column is required.
- **id**: integer, the id of the record if available -- this can be relevant when identifying records after evaluation. Defaults to :python:`range(0,len(data))` if missing.
- **score**: double, the score corresponding to the given record. This could be for example the rating of the item in the case of explicit recommendation. Defaults to constant :python:`1`.
- **eval**: boolean, whether to run ranking-evaluation on the record. Defaults to constant :python:`True`.

Our first model
---------------

Let's start by evaluating a very basic model on the dataset, the popularity model. To do this, we need to import the preconfigured experiment from the package :py:mod:`alpenglow.experimens`.

.. code-block:: python

	from alpenglow.experiments import PopularityModelExperiment

When creating an instance of the experiment, we can provide various configuration options and parameters.

.. code-block:: python

	pop_experiment = PopularityModelExperiment(
	    topK=100, # we are going to evaluate on top 100 ranking lists
	    seed=12345, # for reproducibility, we provide a random seed
	)

You can see the list available options in the documentation of the :py:class:`alpenglow.OnlineExperiment` class and the specific implementation (in this case, the :py:class:`alpenglow.experiments.PopularityModelExperiment` class) or, failing that, in the source code of the given class.

Running the experiment on the data is as simple as calling :python:`run()`. Multiple options can be provided at this point, for a full list, refer to the documentation of :py:meth:`alpenglow.OnlineExperiment.OnlineExperiment.run`.

.. code-block:: python

	result = pop_experiment.run(data, verbose=True) #this might take a while

The :python:`run()` method first builds the experiment out of C++ components according to the given parameters, then processes the data, training and evaluating at the same time. The returned object is a :py:class:`alpenglow.cpp.RankingLogs` object, which contains various information regarding the results of the experiment. For example, we could get the list of ranks corresponding to the rows with :python:`eval=1` using :python:`ranks = [i.rank for i in result.logs]`.

However, the easiest way interpret the results is by using a prebuild evaluator, for example :py:class:`alpenglow.NdcgScore.NdcgScore`:


.. code-block:: python

	import alpenglow as ag
	scores = ag.NdcgScore(results).time_frame(60 * 60 * 24)

The :py:class:`NdcgScore` class calculates the NDCG values for the given ranks, which can then be acquired by accessing the :python:`ndcgs` member of the created instance, or as in the above example, further processed by calculating the average over specific timeframes. The :py:meth:`alpenglow.NdcgScore.NdcgScore.time_frame` method returns a pandas DataFrame containing the averages -- this can be plotted easily to visualize the performance of the recommender model.


.. code-block:: python

	scores.plot()

[todo plot]

Putting it all tohether:

.. code-block:: python

	import pandas as pd
	import alpenglow as ag
	from alpenglow.experiments import PopularityModelExperiment

	data = pd.read_csv('artist_data_10_1800')

	pop_experiment = PopularityModelExperiment(
	    topK=100,
	    seed=12345,
	)
	result = pop_experiment.run(data, verbose=True)
	scores = ag.NdcgScore(results).time_frame(60 * 60 * 24)
	scores.plot()
