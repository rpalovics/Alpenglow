import alpenglow.Getter as rs
import alpenglow as prs

class SimulatedBatchOnlineExperiment(prs.OnlineExperiment):
    def config(self, elems):
        proceeding_logger = rs.ProceedingLogger()
        proceeding_logger.set_data_iterator(elems['randomAccessIterator'])
        config = self.parameterDefaults(
            topK= 100,
            minTime= 0
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

        batch_learner = rs.OfflineImplicitGradientLearner(**self.parameterDefaults(
            number_of_iterations=3,
            start_time=-1,
            period_length=86400,
            write_model=False,
            read_model=True,
            clear_model=False,
            learn=False,
            base_out_file_name="",
            base_in_file_name=""
        ))
        batch_learner.set_model(model)
        batch_learner.set_recommender_data_iterator(elems['randomAccessIterator'])
        batch_learner.add_gradient_updater(updater)


        online_learner = rs.ImplicitGradientLearner()
        online_learner.set_train_matrix(elems['trainMatrix'])
        online_learner.add_gradient_updater(updater)
        online_learner.set_model(model)

        learner = rs.CombinedDoubleLayerLearner()
        learner.addLearner(batch_learner)
        learner.addLearner(online_learner)

        negativeSampleGenerator = rs.UniformNegativeSampleGenerator(**self.parameterDefaults(
            negativeRate=100,
            initializeAll=False,
        ))
        negativeSampleGenerator.setTrainMatrix(elems['trainMatrix'])
        negativeSampleGenerator.setItems(elems['items'])
        online_learner.set_negative_sample_generator(negativeSampleGenerator)

        pointWise = rs.ObjectiveMSE()
        gradientComputer = rs.GradientComputerPointWise(pointWise)
        gradientComputer.set_model(model)
        online_learner.set_gradient_computer(gradientComputer)
        
        return {
            'config': config,
            'model': model,
            'learner': learner
        }