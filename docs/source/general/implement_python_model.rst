Implementing a new model in Python
==================================

.. role:: python(code)
   :language: python

While the core of the framework runs in C++, the fact that Alpenglow uses `SIP <https://www.riverbankcomputing.com/static/Docs/sip/>`_ for its Python bindings allows us to implement models in Python, inheriting from the necessary C++ classes. Please note though that this feature is still experimental and may be a little rough arond the edges.

Let's use a very simple example for demonstration: the empirical transition probability model. This model records how often items follow each other, and always recommenders the empirically most likely next item based on this log. Note that even though the name implies that our model should output probabilities, in fact outputting raw counts is the same from an evaluation perspective, since the empirical probability is monotonic as a function of counts.

The following code demonstrates a very simple implementation of this:


.. code-block:: python

    import pandas as pd
    from collections import defaultdict

    from alpenglow.evaluation import DcgScore
    from alpenglow import SelfUpdatingModel, OnlineExperiment
        

    data = pd.read_csv("http://info.ilab.sztaki.hu/~fbobee/alpenglow/alpenglow_sample_dataset")


    class TransitionProbabilityModel(SelfUpdatingModel):
        def __init__(self):
            super(TransitionProbabilityModel, self).__init__()
            self.last_item = defaultdict(lambda: -1)
            self.num_transitions = defaultdict(lambda: 0)

        def update(self, rec_dat):
            self.num_transitions[(self.last_item[rec_dat.user], rec_dat.item)] += 1
            self.last_item[rec_dat.user] = rec_dat.item
        
        def prediction(self, rec_dat):
            return self.num_transitions[(self.last_item[rec_dat.user], rec_dat.item)]


    class TransitionProbabilityExperiment(OnlineExperiment):
        def _config(self, top_k, seed):
            model = TransitionProbabilityModel()
            return (model._model, model._updater, [])

    experiment = TransitionProbabilityExperiment(top_k=5)
    rankings = experiment.run(data.head(100000))
    rankings['dcg'] = DcgScore(rankings)
    averages = rankings['dcg'].groupby((rankings['time']-rankings['time'].min())//86400).mean()
    print(averages)

We import the neccessary packages, load the data, define the model and define the experiment. The model definition is done by subclassing :py:class:`alpenglow.SelfUpdatingModel`. Note, that this itself is not a C++ class, but a Python class that handles a few neccessary steps for us. It is possible to go deeper and implement the model and its updater separately for example. For this and other, more fine-grained possibilities, please refer to the source of :py:class:`alpenglow.SelfUpdatingModel` and the page :ref:`cpp_api`.

We define three functions for the model: initialization, update and prediction. Initialization is self-explanatory. Update is called after each evaluation step, and receives the training sample as parameter. For the definition of the type of rec_dat, please refer to :py:class:`alpenglow.cpp.RecDat`. Prediction is called for scoring positive samples, as well as to determine the ranking of items during evaluation.

The implemented logic in the above example is quite simple: we store two dictionaries - one contains the last visited items of each user, the other counts the number of occurrences of items after each other. The prediction is simply the latter number.

Let's run the experiment:

::

    :$ time python first_example.py
    running experiment...
    0%-1%-2%-3%-4%-5%-6%-7%-8%-9%-10%-11%-12%-13%-14%-15%-16%-17%-18%-19%-20%-21%-22%-23%-24%-25%-26%-27%-28%-29%-30%-31%-32%-33%-34%-35%-36%-37%-38%-39%-40%-41%-42%-43%-44%-45%-46%-47%-48%-49%-50%-51%-52%-53%-54%-55%-56%-57%-58%-59%-60%-61%-62%-63%-64%-65%-66%-67%-68%-69%-70%-71%-72%-73%-74%-75%-76%-77%-78%-79%-80%-81%-82%-83%-84%-85%-86%-87%-88%-89%-90%-91%-92%-93%-94%-95%-96%-97%-98%-99%-OK
    time
    0.0    0.003957
    1.0    0.004454
    2.0    0.006304
    3.0    0.006426
    4.0    0.009281
    Name: dcg, dtype: float64

    real    2m50.635s
    user    2m56.236s
    sys     0m10.479s

We can see that the score is nicely improving from week to week: the model is able to learn incrementally. We can compare it to the builtin transition model:

::

    :$ time python transition_builtin.py 
    running experiment...
    0%-1%-2%-3%-4%-5%-6%-7%-8%-9%-10%-11%-12%-13%-14%-15%-16%-17%-18%-19%-20%-21%-22%-23%-24%-25%-26%-27%-28%-29%-30%-31%-32%-33%-34%-35%-36%-37%-38%-39%-40%-41%-42%-43%-44%-45%-46%-47%-48%-49%-50%-51%-52%-53%-54%-55%-56%-57%-58%-59%-60%-61%-62%-63%-64%-65%-66%-67%-68%-69%-70%-71%-72%-73%-74%-75%-76%-77%-78%-79%-80%-81%-82%-83%-84%-85%-86%-87%-88%-89%-90%-91%-92%-93%-94%-95%-96%-97%-98%-99%-OK
    time
    0.0    0.002760
    1.0    0.003982
    2.0    0.005773
    3.0    0.006265
    4.0    0.009061
    Name: dcg, dtype: float64

    real    0m5.217s
    user    0m20.329s
    sys     0m3.401s

There are two things to note here. First, the scores are slightly worse. The reason for this is that our implementation implicitly handles cold-start user cases to some degree: we predict the score for the nonexistent previous item with id -1, which basically learns to predict based on item popularity. The builtin model doesn't do this - but this effect is only significant in the very beginning of usual data timelines (and is achievable via model combination using builtin models).

The second thing to note is speed: the builtin experiment runs about 35x faster. This is in part due to the fact that it's implemented in C++ rather than Python - but also due to the fact that it implements something called a *ranking score iterator*. We'll learn more about this in the next section.

.. Note::
    The first time an item is seen in the timeline, it is always because a user just interacted with it for the first time, thus we know that it is in fact a positive sample. If the model for some reason gives higher scores for new items, this could lead to misleading results. In our experience, unfortunately, this happens sometimes unintentionally. To avoid it, the first time an item is seen, the system always returns zero for the ranking. It is thus not possible right now to evaluate completely cold-start item situations. An optional flag is planned for future versions of Alpenglow to selectively re-allow evaluating these records.


Speeding up the evaluation: ranking iterators
#############################################

One way to learn about ranking iterators is to read :ref:`ranking_optimization`. However, let's do a quick recap here as well.

When Alpenglow evaluates a record in the timeline, first it asks the model for a prediction for the given (user, item) pair. Then, to determine the rank of the positive item, it starts asking the model for predictions for other items and counts larger, smaller and equal scores. When the number of larger scores is more than the given top K value we are evaluation for, this process stops: the positive item is not on the toplist. This method has the advantage that it is usually much faster than evaluating on all items.

However, it can be made even faster: the model may be able to give hints about items with larger scores, so that the evaluation might stop faster. This can be done in Python models as well, by defining a *prediction_iterator* method. Let's see an example of this:


.. code-block:: python

    class TransitionProbabilityModel(SelfUpdatingModel):
        def __init__(self):
            super(TransitionProbabilityModel, self).__init__()
            self.last_item = defaultdict(lambda: -1)
            self.transitions = defaultdict(lambda: 0)
            self.nonzero_transitions = defaultdict(lambda: set())
            self.itemset = set()

        def update(self, rec_dat):
            self.transitions[(self.last_item[rec_dat.user], rec_dat.item)] += 1
            self.nonzero_transitions[self.last_item[rec_dat.user]].add(rec_dat.item)
            self.last_item[rec_dat.user] = rec_dat.item
            self.itemset.add(rec_dat.item)
        
        def prediction(self, rec_dat):
            return self.transitions[(self.last_item[rec_dat.user], rec_dat.item)]

        def prediction_iterator(self, user, bound):
            nonzero_pred_items = self.nonzero_transitions[self.last_item[user]]
            for i in self.nonzero_transitions[self.last_item[user]]:
                yield (i, self.transitions[(self.last_item[user], i)])
            
            remaining_items = self.itemset - nonzero_pred_items
            for i in remaining_items:
                if bound() > 0:
                    break
                yield (i, 0)

The main difference from the previous one is the fact that our model now has an additional method, which is actually a generator. This iterates over all of the items that the model is aware of and produces item-score tuples. However, the items with nonzero scores are listed first.

There's one more very important part: the bound parameter of the method. This receives a function that always returns the score under which we are no longer interested in listing the items. I.e. if the bound is 1.0 and somehow we can guarantee that all the remaining items have a score below 1.0, we can stop iterating. When simply running an experiment this stays constant - the score of the positive item. However, in other cases, such as when the toplists are actually calculated, it may change based on the progress of the calculation.

We could further optimize this function by first sorting the nonzero transitions, but the above implementation already achieves a significant speedup:

::

    :$ time python first_example.py 
    running experiment...
    0%-1%-2%-3%-4%-5%-6%-7%-8%-9%-10%-11%-12%-13%-14%-15%-16%-17%-18%-19%-20%-21%-22%-23%-24%-25%-26%-27%-28%-29%-30%-31%-32%-33%-34%-35%-36%-37%-38%-39%-40%-41%-42%-43%-44%-45%-46%-47%-48%-49%-50%-51%-52%-53%-54%-55%-56%-57%-58%-59%-60%-61%-62%-63%-64%-65%-66%-67%-68%-69%-70%-71%-72%-73%-74%-75%-76%-77%-78%-79%-80%-81%-82%-83%-84%-85%-86%-87%-88%-89%-90%-91%-92%-93%-94%-95%-96%-97%-98%-99%-OK
    time
    0.0    0.003903
    1.0    0.004307
    2.0    0.006239
    3.0    0.006659
    4.0    0.009002
    Name: dcg, dtype: float64

    real    0m12.604s
    user    0m53.413s
    sys     0m8.367s

That's a nice improvement! Of course, being able to impement an iterator can be useful in other ways as well - for example if the model can more efficiently calculate scores for batches of items, we could first calculate a batch and then yield the scores one at a time.


.. Note::
    Sometimes the results of an experiment can slightly differ after implementing a ranking iterator. This happens because after the number of larger, smaller and equal items is calculated, the evaluator randomly chooses each equally scored item to be either under or above the positive item in the toplist. The randomness for this is consistent across runs based on the seed, but it's unfortunately not consistent between evaluation methods yet.

.. Warning::
    Not listing all the items in the iterator (or erronously stopping too soon based on the bound) could incorrectly produce higher results than it should. Please take extra care when implementing ranking iterators and try to cross-check against the unoptimized version of the same model.


Speeding up the evaluation: toplists
####################################

There's one more optional method for Python models: *get_top_list*. This is also used automatically for speeding up evaluation, and it takes preference over *prediction_iterator*. Below is an example of this.


.. code-block:: python

    class TransitionProbabilityModel(SelfUpdatingModel):
        def __init__(self):
            super(TransitionProbabilityModel, self).__init__()
            self.last_item = defaultdict(lambda: -1)
            self.transitions = defaultdict(lambda: 0)
            self.nonzero_transitions = defaultdict(lambda: set())
            self.itemset = set()

        def update(self, rec_dat):
            self.transitions[(self.last_item[rec_dat.user], rec_dat.item)] += 1
            self.nonzero_transitions[self.last_item[rec_dat.user]].add(rec_dat.item)
            self.last_item[rec_dat.user] = rec_dat.item
            self.itemset.add(rec_dat.item)
        
        def prediction(self, rec_dat):
            return self.transitions[(self.last_item[rec_dat.user], rec_dat.item)]

        def get_top_list(self, user, k, exclude):
            last_item = self.last_item[user]
            nonzero = self.nonzero_transitions[last_item]
            nonzero_tuples = [(i, self.transitions[(last_item, i)]) for i in nonzero if not i in exclude]
            sorted_nonzero = sorted(nonzero_tuples, key=lambda x: x[1], reverse=True)
            return sorted_nonzero[:k]

The idea is pretty straightforward: we implement a get_top_list method that return a list of (item, score) pairs of length k, in descending order of rank. The parameter *exclude* is used to provide the model with information about items that should be excluded from the toplist. This is used for example when :python:`exclude_known=True`.

::

    :$ time python toplist_example.py 
    running experiment...
    0%-1%-2%-3%-4%-5%-6%-7%-8%-9%-10%-11%-12%-13%-14%-15%-16%-17%-18%-19%-20%-21%-22%-23%-24%-25%-26%-27%-28%-29%-30%-31%-32%-33%-34%-35%-36%-37%-38%-39%-40%-41%-42%-43%-44%-45%-46%-47%-48%-49%-50%-51%-52%-53%-54%-55%-56%-57%-58%-59%-60%-61%-62%-63%-64%-65%-66%-67%-68%-69%-70%-71%-72%-73%-74%-75%-76%-77%-78%-79%-80%-81%-82%-83%-84%-85%-86%-87%-88%-89%-90%-91%-92%-93%-94%-95%-96%-97%-98%-99%-OK
    time
    0.0    0.003675
    1.0    0.004191
    2.0    0.006286
    3.0    0.006221
    4.0    0.009494
    Name: dcg, dtype: float64

    real    0m49.867s
    user    1m1.892s
    sys     0m4.906s

Faster than the first version, slower than ranking iterators. This makes sense: while ranking iterators may stop early, creating the toplist is slower as it always considers all nonzero items. Moreover, the above implementation is not optimal: we could either keep the items in a priority list for each user, or simply do an O(n) top k selection instead of sorting. Another improvement we could make is to complete the toplist when it's too short or break ties using e.g. popularity.

.. Note::
    Once again the result is different. This is, again, due to equally scored items. In toplist models, it's the responsibility of the model to handle this question correctly. Note though that the effect of equally scored items is unusually strong in case of the transition probability model, and is much less pronounced in others, such as matrix factorization.