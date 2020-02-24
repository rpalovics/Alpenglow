Composite classes
=================

Goals of composite classes:

* provide modularity, e.g. make possible replacing :py:class:`alpenglow.cpp.UniformNegativeSampleGenerator` in a preconfigured factor model experiment to another negative sample generator class
* provide preconfigured recommender models (model+updater+accessories like negsamplegenerators, objective, gradient computer if necessary) for model combination (and possibly also for embedding online recommenders in offline framework and vice versa).

Sample codes
------------

Replaceable parts: create poptf recommender using pop.

.. code-block:: python

    from alpenglow.Getter import Getter as cpp

    # create a recipe...
    bestpop_recommender = alpenglow.PopularityModelComposite()
    # for the following object graph:
    bestpop_recommender.show_graph()
    # Output:
    #      ----------------------- 
    #     | model:PopularityModel |
    #      -----------------------
    #               ^
    #               |
    #               | set_model
    #               |
    #  --------------------------------
    # | updater:PopularityModelUpdater |
    #  --------------------------------
    # Such a graph should be the documentation for each composite.

    # The types are default types, each object can be replaced:
    my_updater = cpp.PopularityTimeframeModelUpdater(
      time_frame = 86400
    )
    bestpop_recommender.set_object("updater", my_updater)
    bestpop_recommender.show_graph()
    # Output:
    #      ----------------------- 
    #     | model:PopularityModel |
    #      -----------------------
    #               ^
    #               |
    #               | set_model
    #               |
    #  --------------------------------------------------------
    # | updater:PopularityTimeframeModelUpdater (instantiated) |
    #  --------------------------------------------------------

    # instantiate the missing objects (here the model) and call
    # set_xxx() functions:
    bestpop_recommender.build()

Creating combined model.

.. code-block:: python
    
    recommender1 = alpenglow.PopularityModelComposite()
    recommender1.build()
    recommender2 = alpenglow.TransitionModelComposite()
    recommender2.build()
    # composites can have parameters:
    recommender3 = alpenglow.FactorModelComposite(
        learning_rate = 0.1,
        negative_rate = 100
    )
    recommender3.build()

    #assuming no CombinedModelComposite is available...
    model = cpp.CombinedModel()
    model.add_model(recommender1.get_object("model"))
    model.add_model(recommender2.get_object("model"))
    model.add_model(recommender3.get_object("model"))

    #creating further parts of the combined model
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
    
    #online_experiment
    online_experiment = cpp.OnlineExperiment(
        random_seed=12345,
        top_k=100,
        exclude_known=True,
        initialize_all=False
    )
    online_experiment.add_updater(negative_sample_generator_c) #this will be called first
    online_experiment.add_updater(recommender1.get_object("updater"))
    online_experiment.add_updater(recommender2.get_object("updater"))
    online_experiment.add_updater(recommender3.get_object("updater"))

    #evaluation
    online_experiment.add_logger(logger1)
    logger1 = cpp.MemoryRankingLogger(
        memory_log = True
    )
    logger1.set_model(model)
    ranking_logs = cpp.RankingLogs()
    ranking_logs.top_k = 100
    logger1.set_ranking_logs(ranking_logs)
    

Simplifying things: even CombinedModel could have a composite.

.. code-block:: python
    
    recommender1 = alpenglow.PopularityModelComposite()
    recommender1.build()
    recommender2 = alpenglow.TransitionModelComposite()
    recommender2.build()
    recommender3 = alpenglow.FactorModelComposite()
    recommender3.build()

    recommender = alpenglow.CombinedModelComposite()
    recommender.build() #instantiates model, updater, negsamplegen etc. inside

    model = recommender.get_object("model")
    model.add_model(recommender1.get_object("model"))
    model.add_model(recommender2.get_object("model"))
    model.add_model(recommender3.get_object("model"))
    
    #online_experiment (we could maybe create OnlineExperimentComposite or OnlineExperimentWithTypicalLoggersComposite)
    online_experiment = cpp.OnlineExperiment(
        random_seed=12345,
        top_k=100,
        exclude_known=True,
        initialize_all=False
    )
    online_experiment.add_updater(recommender.get_object("updater"))
    online_experiment.add_updater(recommender1.get_object("updater"))
    online_experiment.add_updater(recommender2.get_object("updater"))
    online_experiment.add_updater(recommender3.get_object("updater"))

    #evaluation
    logger1 = cpp.MemoryRankingLogger(
        memory_log = True
    )
    logger1.set_model(model)
    ranking_logs = cpp.RankingLogs()
    ranking_logs.top_k = 100
    logger1.set_ranking_logs(ranking_logs)
    online_experiment.add_logger(logger1)

Problematic cases
-----------------

Replacing dependent parts:

.. code-block:: python

    recommender = alpenglow.FactorModelComposite()
    recommender.draw_graph()
    # Output:
    #      ------------------- 
    #     | model:FactorModel |
    #      -------------------
    #               ^
    #               | set_model
    #               |
    #  ---------------------------------------------
    # | gradient_updater:FactorModelGradientUpdater |
    #  ---------------------------------------------
    #               ^
    #               | set_gradient_updater
    #               |
    #  ---------------------------   set_objective   --------------
    # | gradient_computer:        |---------------->| objective:   |
    # | GradientComputerPointWise |                 | ObjectiveMSE |
    #  ---------------------------                   --------------
    #               ^
    #               | set_updater
    #               |
    #  --------------------------------
    # | negative_sample_generator:     |
    # | UniformNegativeSampleGenerator |
    #  --------------------------------

    my_objective = cpp.ObjectiveBPR() #pairwise objective
    recommender.set_object("objective",my_objective)

    #Difficulty: GradientComputerPointWise::set_objective() expects a pointwise objective
    #---> we need to replace it too
    #that could be automatic, but it's not trivial, how

    my_gradient_computer = cpp.GradientComputerPairWise()
    recommender.set_object("gradient_computer",my_gradient_computer)

    recommender.build() #will call my_gradient_computer.set_objective(my_objective)?
    #or we should call it, because we replaced both ends of the edge?
    #How could we handle the case if the funtion name differs (e.g. set_pairwise_objective)?
    #---->we need an option to disable set_xxx calls, but there might be repeating names
    #---->gradient_computer.set_objective will be a unique name

Multiple names for the same object to make automatic wiring between composites possible:

.. code-block:: python

    recommender = alpenglow.FactorModelComposite()
    recommender.build()

    experiment = alpenglow.OnlineExperimentWithTypicalLoggersCompsite()
    experiment.set_recommender(recommender) #takes "model", "updater"
    #and "logger" from recommender and sets them appropriately

    #Seems straightforward... but there is no "updater" object:
    recommender.draw_graph()
    # Output:
    #      ------------------- 
    #     | model:FactorModel |
    #      -------------------
    #               ^
    #               | set_model
    #               |
    #  ---------------------------------------------
    # | gradient_updater:FactorModelGradientUpdater |
    #  ---------------------------------------------
    #               ^
    #               | set_gradient_updater
    #               |
    #  ---------------------------   set_objective   --------------
    # | gradient_computer:        |---------------->| objective:   |
    # | GradientComputerPointWise |                 | ObjectiveMSE |
    #  ---------------------------                   --------------
    #               ^
    #               | set_updater
    #               |
    #  --------------------------------
    # | negative_sample_generator:     |
    # | UniformNegativeSampleGenerator |
    #  --------------------------------

    #Solution1: negsamplegen could have a second name
    #but what if we have multiple updaters or loggers?
    #Better solution: define get_model, get_updaters, get_loggers functions

Name
----

Composite is a nice name, but it might lead to confusion (there is a well-known Composite desing pattern, but our composite does not implement that pattern). Alternative ideas:

* Szerelvény/Assembly (mint a vasuti szerelveny, pl. a RailJet egy szerelveny, van rajta mozdony, kocsik, etkezokocsi stb., de ki lehet benne cserelni egy kocsit vagy lecserelni a mozdonyt)
* ObjectGraph
* Component; PrebuiltComponent, WiredComponent, AutoWiringComponent
* ???
