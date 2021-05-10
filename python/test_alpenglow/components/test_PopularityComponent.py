import alpenglow.components.PopularityComponent
import alpenglow.Getter as ag
import pytest

class TestPopularityComponent:
    def test_pop_component(self):
        c = alpenglow.components.PopularityComponent()
        c.show()
        c.build()

    def test_poptf(self):
        c = alpenglow.components.PopularityComponent()
        poptf_updater = ag.PopularityTimeFrameModelUpdater(
            tau = 86400
        )
        c.set_object("updater",poptf_updater)
        with pytest.raises(RuntimeError):
          c.set_object("invalid_name",poptf_updater) #invalid object name
        c.get_object("updater") #ok, already set
        with pytest.raises(RuntimeError):
          c.get_object("model") #does not exist yet

        c.build()

        with pytest.raises(RuntimeError):
          c.set_object("updater",poptf_updater) #already built
        c.get_object("model") #ok, already created
