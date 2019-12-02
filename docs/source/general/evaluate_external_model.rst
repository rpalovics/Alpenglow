Evaluating external models
==========================

The goal of Alpenglow is to evaluate models in an on-line recommendation setting. This is primarily done through implementing the models in either C++ or Python and running them within Alpenglow experiments. However, there's also a builtin way to evaluate external models as periodically trained models in the same setting. This works by preparing an experiment that writes training files to disk, running the external model to train on these and predict toplists for given users, then run another experiment that reads these toplists back and provides them as predictions.

This logic is implemented thorugh :py:class:`alpenglow.experiments.ExternalModelExperiment`. Below is an example of an experiment that prepares the training data:

.. code-block:: python

    exp = ExternalModelExperiment(
        period_length=60 * 60 * 24 * 7 * 4,
        out_name_base="batches/batch",
        mode="write"
    )

When run, this experiment creates files such as *batches/batch_1_train.dat* and *batches/batch_1_test.dat*. The first is a CSV containing the training data, the second is a list of users that the model should generate toplists for. The predictions themselves should be saved in the file *batches/batch_1_predictions.dat* as CSV, containing 'user', 'item' and 'pos' columns. Then, the following code can be used to evaluate the results:

.. code-block:: python

    exp = ExternalModelExperiment(
        period_length=60 * 60 * 24 * 7 * 4,
        in_name_base="batches/batch",
        mode="read",
    )

For working examples, please check out the `examples/external_models <https://github.com/rpalovics/Alpenglow/tree/master/examples/external_models>`_ directory of the repository, where this process is demonstrated through multiple examples, such as `LibFM <https://github.com/srendle/libfm>`_, `LightFM <https://github.com/lyst/lightfm>`_ and `Turicreate <https://github.com/apple/turicreate>`_.