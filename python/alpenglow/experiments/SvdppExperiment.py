import alpenglow.Getter as rs
import alpenglow as prs


class SvdppExperiment(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameter_defaults(
            top_k=100,
            min_time=0,
            seed=67439852,
            out_file=None,
            filters=[],
            loggers=[],
        )

        model = rs.SvdppModel(**self.parameter_defaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            use_sigmoid=False,
            norm_type="exponential",
            gamma=0.8,
            user_vector_weight=0.5,
            history_weight=0.5
        ))

        gradient_updater = rs.SvdppModelGradientUpdater(**self.parameter_defaults(
            learning_rate=0.05,
            cumulative_item_updates=False,
        ))
        gradient_updater.set_model(model)
        simple_updater = rs.SvdppModelUpdater()
        simple_updater.set_model(model)

        learner = rs.ImplicitGradientLearner()
        learner.add_gradient_updater(gradient_updater)
        learner.add_simple_updater(simple_updater)
        learner.set_model(model)

        negative_sample_generator = rs.UniformNegativeSampleGenerator(**self.parameter_defaults(
            negative_rate=20,
            initialize_all=False,
            seed=928357823,
        ))
        learner.set_negative_sample_generator(negative_sample_generator)

        point_wise = rs.ObjectiveMSE()
        gradient_computer = rs.GradientComputerPointWise()
        gradient_computer.set_objective(point_wise)
        gradient_computer.set_model(model)
        learner.set_gradient_computer(gradient_computer)

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
