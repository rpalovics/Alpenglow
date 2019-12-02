Recommender data
----------------

This module contains the classes that are responsible for reading in the
dataset and serving it to other classes of the experiment.

Interface :py:class:`alpenglow.cpp.RecommenderData` is the anchestor for
classes that read in the dataset.  The two most frequently used implementations
are :py:class:`alpenglow.cpp.DataframeData` and
:py:class:`alpenglow.cpp.LegacyRecommenderData`.

Interface :py:class:`alpenglow.cpp.RecommenderDataIterator` is the anchestor
for classes that serve the data to the classes in the online experiment.  See
:doc:`/general/anatomy_of_experiment` for general information.  The most
frequently used implementations are :py:class:`alpenglow.cpp.ShuffleIterator`
and :py:class:`alpenglow.cpp.SimpleIterator`.

