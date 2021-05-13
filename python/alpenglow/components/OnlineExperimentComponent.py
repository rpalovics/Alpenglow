import alpenglow as prs

class OnlineExperimentComponent(prs.Component):

    def _get_catalog(self):
        catalog = {
          "model" : {
            "type" : "PopularityModel",
            "object" : None,
            "parameters" : {},
            "connections" : []
          },
          "updater" : {
            "type" : "PopularityModelUpdater",
            "object" : None,
            "parameters" : {},
            "connections" : [
              ("set_model", "model")
            ]
          }
        }
        return catalog

    def set_model(self,model):
        self.model = model
    
    def set_updater(self,updater):
        self.updater = updater

    def run(self, data, verbose, exclude_known):
       return 0
