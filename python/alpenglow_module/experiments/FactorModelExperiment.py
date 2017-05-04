import alpenglow.Getter as rs
import alpenglow as prs


class FactorModelExperiment(prs.OnlineExperiment):
    def config(self, elems):
        proceeding_logger = rs.ProceedingLogger()
        proceeding_logger.set_data_iterator(elems['randomAccessIterator'])
        config = self.parameterDefaults(
            topK=100,
            minTime=0,
            seed=0,
            outFile=None,
            filters=[],
        )
        config['loggers'] = [proceeding_logger] if self.verbose else []

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

        learner = rs.ImplicitGradientLearner()
        learner.set_train_matrix(elems['trainMatrix'])
        learner.add_gradient_updater(updater)
        learner.set_model(model)

        negativeSampleGenerator = rs.UniformNegativeSampleGenerator(**self.parameterDefaults(
            negativeRate=0.0,
            initializeAll=False,
            seed=0,
        ))
        negativeSampleGenerator.setTrainMatrix(elems['trainMatrix'])
        negativeSampleGenerator.setItems(elems['items'])
        learner.set_negative_sample_generator(negativeSampleGenerator)

        pointWise = rs.ObjectiveMSE()
        gradientComputer = rs.GradientComputerPointWise(pointWise)
        gradientComputer.set_model(model)
        learner.set_gradient_computer(gradientComputer)

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
