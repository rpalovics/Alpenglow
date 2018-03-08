import alpenglow as ag
import alpenglow.Getter as rs
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

data = pd.read_csv(
    "/mnt/idms/recSys/data/30M_lastfm/recoded_online_id_artist_first_filtered",
    sep=' ',
    header=None,
    names=['time', 'user', 'item', 'id', 'score', 'eval'],
    nrows=100000
)

class MyExperiment(ag.OnlineExperiment):
    '''
        Sample exponentially weighted forecaster experiment. The experiment combined four models, a popularity model, a factor model and two models that predict always 0.
        The exponentially weighted forecaster algorithm is described in
        Stoltz, G. 2005. Incomplete information and internal regret in prediction of individual sequences. Ph.D. Dissertation, Chapter 2.
    '''
    def _config(self, top_k, seed):
        model = rs.RandomChoosingCombinedModel()
        updater = rs.RandomChoosingCombinedModelExpertUpdater(**self.parameter_defaults(
            eta=0.1,
            top_k=top_k,
            loss_type="abs",
        ))
        updater.set_model(model)
        pop_model = rs.PopularityModel()
        model.add_model(pop_model)
        pop_updater = rs.PopularityModelUpdater()
        pop_updater.set_model(pop_model)

        pop_model2 = rs.PopularityModel() #not updated popularity model, predicts 0 for all items and users
        model.add_model(pop_model2)
        pop_model3 = rs.PopularityModel() #not updated popularity model, predicts 0 for all items and users
        model.add_model(pop_model3)

        factor_model = rs.FactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
        ))
        model.add_model(factor_model)

        factor_updater = rs.FactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=0.05,
            regularization_rate=0.0
        ))
        factor_updater.set_model(factor_model)

        objective = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(objective)
        gradient_computer.set_model(factor_model)
        gradient_computer.add_gradient_updater(factor_updater)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=10,
            initialize_all=False,
            seed=67439852,
            filter_repeats=False,
        ))
        negative_sample_generator.add_updater(gradient_computer)

        return (model, [updater, pop_updater, negative_sample_generator], [], [])

experiment = MyExperiment(top_k=100, seed=254938879)
rankings = experiment.run(data, verbose=True)
rankings['dcg'] = ag.evaluation.DcgScore(rankings)
day_groups = (rankings['time']-rankings['time'].min())//86400
daily_avg = rankings['dcg'].groupby(day_groups).mean()
plt.figure()
daily_avg.plot()
plt.savefig("exp_weighted_experiment.png")

