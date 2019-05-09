Model combination
=================

While some model combination methods are implemented in Alpenglow, there are no preconfigured combined experiments.
Here is an example that contains the linear combination of three models.
The combination weights are trained with SGD.

.. image:: class_diagram_combined_color.png

The code below is quite long and building experiments this way is error-prone, but currently no graphical building tool is implemented.
The typical fault is to miss some :py:meth:`add_xxxx()` or :py:meth:`set_xxxx()`.
Sometimes the result is blatantly invalid and catched by the :py:meth:`self_test()` call (see the last few lines).
However, sometimes you can end up with hard-to-debug segfaults or invalid results.

The order of :py:meth:`online_experiment.add_updater()` calls is important.
In the updating phase, the order of :py:meth:`update()` calls is indentical to the order here.
This way the combination weights are updated first, then the individual models.

.. code-block:: python

    from alpenglow.Getter import Getter as cpp
    import alpenglow
    import pandas as pd
    
    
    cpp.collect() #see general/memory usage
    
    #data
    data_python = pd.read_csv("http://info.ilab.sztaki.hu/~fbobee/alpenglow/alpenglow_sample_dataset", nrows=2000)
    data_cpp_bridge = alpenglow.DataframeData(data_python)
    data = cpp.ShuffleIterator(seed=12345)
    data.set_recommender_data(data_cpp_bridge)
    
    #recommender1: model+updater
    model1 = cpp.TransitionProbabilityModel()
    updater1 = cpp.TransitionProbabilityModelUpdater(
      mode="normal"
    )
    updater1.set_model(model1)
    
    #recommender3: model+updater
    model3 = cpp.PopularityModel()
    updater3 = cpp.PopularityTimeFrameModelUpdater(
        tau = 86400
    )
    updater3.set_model(model3)
    
    #recommender2:
    model2 = cpp.FactorModel(
      dimension = 10,
      begin_min = -0.1,
      begin_max = 0.1
    )
    negative_sample_generator_f = cpp.UniformNegativeSampleGenerator(
      negative_rate = 10
    )
    gradient_computer_f = cpp.GradientComputerPointWise()
    gradient_computer_f.set_model(model2)
    gradient_updater_f = cpp.FactorModelGradientUpdater(
      learning_rate = 0.08,
      regularization_rate = 0.0
    )
    gradient_updater_f.set_model(model2)
    gradient_computer_f.add_gradient_updater(gradient_updater_f)
    objective_f = cpp.ObjectiveMSE()
    gradient_computer_f.set_objective(objective_f)
    
    #recommender: combined model
    model = cpp.CombinedModel(
      log_file_name="xxx",
      log_frequency=1000000,
      use_user_weights=False
    )
    model.add_model(model1)
    model.add_model(model2)
    model.add_model(model3)
    objective_c = cpp.ObjectiveMSE()
    negative_sample_generator_c = cpp.UniformNegativeSampleGenerator(
      negative_rate = 10
    )
    gradient_computer_c = cpp.GradientComputerPointWise()
    gradient_computer_c.set_model(model)
    negative_sample_generator_c.add_updater(gradient_computer_c)
    gradient_updater_c = cpp.CombinedDoubleLayerModelGradientUpdater(
      learning_rate = 0.05
    )
    gradient_computer_c.add_gradient_updater(gradient_updater_c)
    gradient_computer_c.set_objective(objective_c)
    gradient_updater_c.set_model(model)
    
    #loggers: evaluation&statistics
    logger1 = cpp.MemoryRankingLogger(
        memory_log = True
    )
    logger1.set_model(model)
    ranking_logs = cpp.RankingLogs()
    ranking_logs.top_k = 100
    logger1.set_ranking_logs(ranking_logs)
    logger2 = cpp.TransitionModelLogger(
        toplist_length_logfile_basename = "test",
        timeline_logfile_name = "log",
        period_length = 100000
    )
    logger2.set_model(model1)
    logger3 = cpp.ProceedingLogger()
    
    #online_experiment
    #Class experiment_environment is created inside.
    online_experiment = cpp.OnlineExperiment(
        random_seed=12345,
        top_k=100,
        exclude_known=True,
        initialize_all=False
    )
    online_experiment.add_logger(logger1)
    online_experiment.add_logger(logger2)
    online_experiment.add_logger(logger3)
    online_experiment.add_updater(negative_sample_generator_c) #this will be called first
    online_experiment.add_updater(updater1)
    online_experiment.add_updater(negative_sample_generator_f)
    online_experiment.add_updater(updater3)
    online_experiment.set_recommender_data_iterator(data)
    
    #clean, initialize, test (see general/cpp api)
    objects = cpp.get_and_clean()
    cpp.set_experiment_environment(online_experiment, objects)
    cpp.initialize_all(objects)
    for i in objects:
        cpp.run_self_test(i)
    
    #run the experiment
    online_experiment.run()
    
    result = logger1.get_ranking_logs()
