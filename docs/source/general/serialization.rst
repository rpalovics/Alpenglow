Serialization
=============

Serialization is parially implemented in the Alpenglow framework.
See the code samples below to discover the current serialization possibilities.

Interfaces for serialization
----------------------------

Many C++ classes have `write(ostream& file)` and `read(istream& file)` functions for serialization.  However, these functions are not available directly through the python interface, and also might left unimplemented by some classes (throwing exceptions).

In case of :py:class:`alpenglow.cpp.Model`, one can use `write(std::string file_name)` and `read(std::string file_name)`.


Serialization of periodically retrained models in the online framework
----------------------------------------------------------------------

Use the parameters `write_model=True` and `base_out_file_name` to write trained models in :py:class:`alpenglow.experiments.BatchFactorExperiment` to disk.  See the example below.  Note that the model output directory (`/path/to/your/output/dir/models/` in the example) must exist or no models will be written out.  The model files will be numbered (e.g. `model_1`, `model_2` etc. in the example).

.. code-block:: python

    from alpenglow.experiments import BatchFactorExperiment
    
    data = "/path/to/your/data"
    out_dir = "/path/to/your/output/dir/"
    
    factor_model_experiment = BatchFactorExperiment(
        out_file=out_dir+"/output_legacy_format",
        top_k=100,
        seed=254938879,
        dimension=10,
        write_model=True,
        base_out_file_name=out_dir+"/models/model",
        learning_rate=0.03,
        number_of_iterations=10,
        period_length=100000,
        period_mode="samplenum",
        negative_rate=30
    )
    
    rankings = factor_model_experiment.run(
        data, exclude_known=True, experimentType="online_id")

You can read back your models using the same class, changing the parameters.  Note that the model size parameters (`dimension`, `period_length`, `period_mode`) must agree.  However, the training parameters (`learning_rate`, `negative_rate`, `number_of_iterations`) may be omitted if `learn` is set to `False`.


.. code-block:: python

    from alpenglow.experiments import BatchFactorExperiment
    
    data = "/path/to/your/data"
    out_dir = "/path/to/your/output/dir/"
    
    factor_model_experiment = BatchFactorExperiment(
        out_file=out_dir+"/output_legacy_format",
        top_k=100,
        seed=254938879,
        dimension=10,
        learn=False,
        read_model=True,
        base_in_file_name=out_dir+"/models/model",
        period_length=100000,
        period_mode="samplenum"
    )
    
    rankings = factor_model_experiment.run(
        data, exclude_known=True, experimentType="online_id")

Alternatively, one could read back the models using :py:class:`alpenglow.experiments.BatchAndOnlineFactorExperiment` and apply online updates on top of the pretrained batch models.


Serialization in offline experiments
------------------------------------

See the example below:

.. code-block:: python

    import pandas as pd
    from alpenglow.offline.models import FactorModel
    import alpenglow.Getter as rs
    
    data = pd.read_csv(
        "/path/to/your/data",
        sep=' ',
        header=None,
        names=['time', 'user', 'item', 'id', 'score', 'eval']
    )
    model = FactorModel(
        factor_seed=254938879,
        dimension=10,
        negative_rate=9,
        number_of_iterations=20,
    )
    model.fit(data)
    
    model.model.write("output_file") #writes model to output_file
    rd = rs.RecDat()
    rd.user = 3
    rd.item = 5
    print("prediction for user=3, item=5:", model.model.prediction(rd))
    
    #model2 must have the same dimension
    model2 = FactorModel(
        factor_seed=1234,
        dimension=10,
        negative_rate=0,
        number_of_iterations=0,
    )
    #to create the inner model but avoid training, we need to run fit()
    #on an empty dataset
    data2=pd.DataFrame(columns=['time', 'user', 'item'])
    model2.fit(data2)
    model2.model.read("output_file") #reads back the same model
    print("prediction for user=3, item=5 using the read-back model:",
        model2.model.prediction(rd))



