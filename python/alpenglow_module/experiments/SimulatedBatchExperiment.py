import alpenglow.Getter as rs
import alpenglow as prs

class SimulatedBatchExperiment(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameterDefaults(
            topK= 100,
            minTime= 0
        )

        model = rs.FactorModel(**self.parameterDefaults(
            beginMin=-0.01,
            beginMax=0.01,
            dimension=10,
            initializeAll=False,
        ))

        updater = rs.FactorModelGradientUpdater(**self.parameterDefaults(
            learningRate=0.05,
            regularizationRate=0.0
        ))
        updater.setModel(model)

        learner = rs.OfflineImplicitGradientLearner(**self.parameterDefaults(
            number_of_iterations=3,
            start_time=-1,
            period_length=86400,
            write_model=False,
            read_model=False,
            clear_model=False,
            learn=True,
            base_out_file_name="",
            base_in_file_name=""
        ))
        learner.set_model(model)
        learner.set_recommender_data_iterator(elems['randomAccessIterator'])
        learner.add_gradient_updater(updater)

        negativeSampleGenerator = rs.UniformNegativeSampleGenerator(**self.parameterDefaults(
            negativeRate=3,
            initializeAll=False,
        ))
        negativeSampleGenerator.setTrainMatrix(elems['trainMatrix'])
        negativeSampleGenerator.setItems(elems['items'])
        learner.set_negative_sample_generator(negativeSampleGenerator)

        pointWise = rs.ObjectiveMSE()
        gradientComputer = rs.GradientComputerPointWise(pointWise)
        gradientComputer.set_model(model)
        learner.set_gradient_computer(gradientComputer)
        learner.init()

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
