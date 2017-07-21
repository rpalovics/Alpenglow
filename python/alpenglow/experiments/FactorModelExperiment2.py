import alpenglow.Getter as rs
import alpenglow as prs


class FactorModelExperiment2(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameter_defaults(
            top_k=100,
            min_time=0,
            seed=0,
            out_file=None,
            filters=[],
            loggers=[],
        )

        model = rs.FactorModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
        ))

        updater = rs.FactorModelGradientUpdater(**self.parameter_defaults(
            learning_rate=0.05,
            regularization_rate=0.0
        ))
        updater.set_model(model)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)
        gradient_computer.add_gradient_updater(updater)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=0.0,
            initialize_all=False,
            seed=0,
            filter_repeats=False,
        ))
        negative_sample_generator.add_updater(gradient_computer)


        return {
            'config': config,
            'model': model,
            'learner': [negative_sample_generator]
        }
