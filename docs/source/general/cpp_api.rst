C++ API
=======

The core of Alpenglow is written in C++.
The C++ code is wrapped in python (see https://riverbankcomputing.com/software/sip/intro).
We instantiate the C++ objects from python and wire them together there.

Below we describe the most important C++ interfaces.
It is recommended to read :doc:`/general/anatomy_of_experiment` first.

Main parts of the experiments
-----------------------------

Model
^^^^^

The recommender model of the experiment.
Models implement the interface :py:class:`alpenglow.cpp.Model` as :py:class:`alpenglow.cpp.RankingLogger` expects this type.
To compute the score of the valid item, we query the score of it and compare its score to that of the other items.

Updater
^^^^^^^

In the online experiment, we iterate on a time series of samples.
The framework processes samples one by one.
After the evaluation, each sample becomes a training sample.
Models are updated incrementally through the :py:class:`alpenglow.cpp.Updater` interface.

Logger
^^^^^^

RecommenderDataIterator
^^^^^^^^^^^^^^^^^^^^^^^

Components for gradient based learning algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Negative sample generators
""""""""""""""""""""""""""

Gradient computers and objectives
"""""""""""""""""""""""""""""""""

General interfaces
------------------

Initializable
^^^^^^^^^^^^^

NeedsExperimentEnvironment
^^^^^^^^^^^^^^^^^^^^^^^^^^

self_test() function
^^^^^^^^^^^^^^^^^^^^

Offline experiments
-------------------
