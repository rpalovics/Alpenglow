import alpenglow as prs

class PopularityComponent(prs.Component):
    """Recommends the most popular item from the set of items seen so far.
    """

    def _get_catalog(self):
        catalog = {
          "model" : {
            "type" : "PopularityModel",
            "object" : None,
            "parameters" : {},
            "connections" : {}
          },
          "updater" : {
            "type" : "PopularityModelUpdater",
            "object" : None,
            "parameters" : {},
            "connections" : {
              "set_model" : "model"
            }
          }
        }
        return catalog
