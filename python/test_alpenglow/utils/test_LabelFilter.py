import alpenglow as prs
import alpenglow.Getter as ag
from alpenglow.utils import AvailabilityFilter
import pandas as pd


class LabelExperiment(prs.OnlineExperiment):
    def _config(self, top_k, seed):
        model = ag.PopularityModel()
        updater = ag.PopularityModelUpdater()
        updater.set_model(model)
        label_filter = ag.LabelFilter(**self.parameter_defaults(
            label_file_name = ""
        ))
        adapter = ag.WhitelistFilter2ModelAdapter()
        adapter.set_model(model)
        adapter.set_whitelist_filter(label_filter)

        return (adapter, [updater, label_filter], [], []) #note: do not forget to add filter as an updater

class TestLabelFilter:
    def test_init(self):
        label_filter_df = pd.DataFrame({'items' : range(10)})
        label_filter_df['label'] = label_filter_df['items'] % 2 #label==parity
        label_filter_df.to_csv(
            "python/test_alpenglow/tmp/test_label_filter_label_file",
            sep=" ", index=False, header=False
        )
        
        experiment = LabelExperiment(
            top_k=100,
            seed=254938879,
            label_file_name=
                "python/test_alpenglow/tmp/test_label_filter_label_file"
        )
        data = pd.DataFrame({
          'time': range(30),
          'item': list(range(10)) #training, all items are new -> rank=NaN
                + list(range(10)) #parity is alternating, no repeating labels -> rank=NaN
                + list(range(0,10,2))*2, #sign is repeating -> rank!=NaN (except for the first sample for the users)
          'user': [0]*10+[1]*10+[2]*5+[4]*5
        })
        rankings = experiment.run(data, verbose=True, exclude_known=True)

        #print(list(rankings["rank"].fillna(-1)))

        assert list(rankings["rank"].fillna(-1)) == [
            -1, -1, -1, -1, -1,    -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1,    -1, -1, -1, -1, -1,
            -1, 2.0, 1.0, 1.0, 1.0,    -1, 3.0, 2.0, 1.0, 1.0
        ]
