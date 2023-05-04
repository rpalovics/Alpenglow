Model combination
=================

While some model combination methods are implemented in Alpenglow, there are no preconfigured combined experiments.
Here is an example that contains the linear combination of three models.
The combination weights are trained with SGD.

The following figure shows the C++ classes in the sample experiment.

.. image:: class_diagram_combined_color.png

When instantiating the classes, we apply pre-wired components.
Each purple box - the recommenders - are available as a component.
The remaining classes are part of :code:`OnlineExperimentComponent` with a few exceptions.
:code:`TransitionModelLogger` is not included in TransitionProbabilityComponent by default.
We create it separately, set the appropriate model into it and add it to the experiment near to the end of the code.
For the sake of the example, we build TimeFramePopularity recommender using PopularityComponent, replacing only the updating mechanism.

.. code-block:: python

    import alpenglow.components.TransitionProbabilityComponent
    import alpenglow.components.FactorComponent
    import alpenglow.components.PopularityComponent
    import alpenglow.components.CombinedComponent
    import alpenglow.components.OnlineExperimentComponent
    import alpenglow.Getter as cpp
    import pandas as pd

    #base recommender 1: transition
    recommender1 = alpenglow.components.TransitionProbabilityComponent(
      mode="inverted"
    )
    recommender1.build()
    model1 = recommender1.get_object("model")
    updater1 = recommender1.get_object("updater")
    transition_logger = cpp.TransitionModelLogger( #additional logger for transition probability model
       toplist_length_logfile_basename = "test",
       timeline_logfile_name = "log",
       period_length = 100000
    )
    transition_logger.set_model(model1)

    #base recommender 2: sgd matrix factorization
    recommender2 = alpenglow.components.FactorComponent(
      learning_rate = 0.1,
      negative_rate = 10
    )
    recommender2.build()
    model2 = recommender2.get_object("model")
    updater2 = recommender2.get_object("updater")

    #base recommender 3: poptf
    recommender3 = alpenglow.components.PopularityComponent()
    updater3 = cpp.PopularityTimeFrameModelUpdater(
        tau = 600
    )
    recommender3.set_object("updater",updater3) #replacing the updater of the component
    recommender3.build()
    model3 = recommender3.get_object("model")

    #recommender: combined model
    recommender = alpenglow.components.CombinedComponent(
        #model
        log_frequency=0, #no logging
        use_user_weights=False,
        #negative sample generator
        negative_rate = 10,
        #gradient updater
        learning_rate = 0.05,
    )
    recommender.add_base_model(model1)
    recommender.add_base_model(model2)
    recommender.add_base_model(model3)
    recommender.build()
    model = recommender.get_object("model")
    updater = recommender.get_object("updater") #alias for negative sample generator

    #online_experiment
    experiment = alpenglow.components.OnlineExperimentComponent(
        top_k = 100,
        seed = 254938879,
    )
    experiment.set_model(model) #the evaluator queries the combined model
    experiment.add_updater(updater) #first the combined model is updated
    experiment.add_updater(updater1) #then all base models
    experiment.add_updater(updater2)
    experiment.add_updater(updater3)
    experiment.add_logger(transition_logger) #adding the extra logger, ProceedingLogger and RankingLogger are included in OnlineExperimentComponent
    experiment.build()

    #data
    data = pd.read_csv("http://info.ilab.sztaki.hu/~fbobee/alpenglow/alpenglow_sample_dataset", nrows=2000)

    #running the experiment
    rankings = experiment.run(data, verbose=True, exclude_known=True)

The code below is quite long and building experiments this way is error-prone, but currently no graphical building tool is implemented.
The typical fault is to miss some :py:meth:`add_xxxx()` or :py:meth:`set_xxxx()` functions.
Sometimes the result is blatantly invalid and catched by the :py:meth:`self_test()` call (this method is automatically called by OnlineExperimentComponent).
However, sometimes you can end up with hard-to-debug segfaults or invalid results.

Take care to add each base model to the combined model, set the combined model to the online experiment and add each updater, starting with the combination updater.

The order of :py:meth:`online_experiment.add_updater()` calls is important.
In the updating phase, the order of :py:meth:`update()` calls is indentical to the order here.
This way the combination weights are updated first, then the individual models.
