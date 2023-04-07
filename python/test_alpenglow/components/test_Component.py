import alpenglow

class TestComponent:
    def test_component(self):
        class DummyComponent(alpenglow.Component):
          def _get_catalog(self):
            catalog = {
              "model" : {
                "type" : "PopularityModel",
                "object" : None,
                "parameters" : {},
                "connections" : []
              }
            }
            return catalog
        c = DummyComponent()
        c.show()
        c.build()
        parameter_container = c.get_parameter_container()

        c2 = DummyComponent(parameter_container)
