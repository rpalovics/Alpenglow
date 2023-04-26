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
              },
              "another_name_for_model" : {
                "alias_for" : "model"
              }
            }
            return catalog
        c = DummyComponent()
        c.show()
        c.build()

        model = c.get_object("model")
        assert isinstance(model,alpenglow.cpp.PopularityModel)

        alias = "another_name_for_model"
        assert c.is_alias(alias)
        assert not c.is_alias("model")

        canonical_name = c.resolve_alias(alias)
        assert canonical_name == "model"

        alias_object = c.get_object("another_name_for_model")
        assert isinstance(alias_object,alpenglow.cpp.PopularityModel)

        parameter_container1 = c.get_parameter_container()

        c2 = DummyComponent(parameter_container=parameter_container1)
        parameter_container2 = c2.get_parameter_container()

        assert parameter_container1==parameter_container2
