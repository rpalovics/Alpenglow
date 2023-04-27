import alpenglow.Getter as cpp
import alpenglow as ag

class CombinedComponent(ag.Component):

    def __init__(self, **parameters):
        super().__init__(**parameters)
        self.models = []

    def add_base_model(self,model):
        if self._built :
          raise RuntimeError("Add base models before building the component.")
        self.models.append(model)

    def build(self):
        super().build()
        model = self.get_object("model")
        for base_model in self.models:
          model.add_model(base_model)

    def _get_catalog(self):
        catalog = {
          "model" : {
            "type" : "CombinedModel",
            "object" : None,
            "parameters" : {
              "log_file_name": "",
              "log_frequency": 0,
              "use_user_weights": False
            },
            "connections" : []
          },
          "gradient_updater" : {
            "type" : "CombinedDoubleLayerModelGradientUpdater",
            "object" : None,
            "parameters" : {
              "learning_rate": 0.05,
              "regularization_rate" : 0.0,
              "global_learning_rate" : 0.05,
              "global_regularization_rate": 0.0,
              "always_learn": True,
              "start_combination_learning_time": 0
              },
            "connections" : [
              ("set_model", "model")
            ]
          },
          "point_wise_objective" : {
            "type" : "ObjectiveMSE",
            "object" : None,
            "parameters" : {},
            "connections" : []
          },
          "point_wise_gradient_computer" : {
            "type" : "GradientComputerPointWise",
            "object" : None,
            "parameters" : {},
            "connections" : [
              ("set_model", "model"),
              ("set_objective", "point_wise_objective"),
              ("add_gradient_updater", "gradient_updater")
            ]
          },
          "negative_sample_generator" : {
            "type" : "UniformNegativeSampleGenerator",
            "object" : None,
            "parameters" : {
              "negative_rate" : 100,
              "initialize_all" : False,
              "seed" : 67439852,
              "filter_repeats" : False
              },
            "connections" : [
              ("add_updater", "point_wise_gradient_computer")
            ]
          },
          "updater" : {
            "alias_for": "negative_sample_generator"
          }
        }
        return catalog
