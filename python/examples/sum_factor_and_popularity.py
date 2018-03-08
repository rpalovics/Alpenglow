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
    def _config(self, top_k, seed):
        model = rs.CombinedModel(**self.parameter_defaults(
            los_file_name="my_log_file",
            log_frequency=100000,
            use_user_weights=False,
        ))
        pop_model = rs.PopularityModel()
        model.add_model(pop_model)
        pop_updater = rs.PopularityModelUpdater()
        pop_updater.set_model(pop_model)

        factor_model = rs.FactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
        ))

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

        return (model, [pop_updater, negative_sample_generator], [], [])

experiment = MyExperiment(top_k=100, seed=254938879)
rankings = experiment.run(data, verbose=True)
rankings['dcg'] = ag.evaluation.DcgScore(rankings)
day_groups = (rankings['time']-rankings['time'].min())//86400
daily_avg = rankings['dcg'].groupby(day_groups).mean()
plt.figure()
daily_avg.plot()
plt.savefig("sumexperiment.png")
