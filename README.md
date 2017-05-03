# Alpenglow
(name might change because of conflict)

### Requirements:
	- Anaconda + python3 environment

### Installation:
	- `cd alpenglow`
	- `conda install libgcc`
	- `pip install .`

### Development:
	- For faster recompilation, use `export CC="ccache cc"`
	- To enable compilation on 4 threads for example, use `echo 4 > .parallel`
	- Reinstall modified version using `pip install --upgrade --force-reinstall --no-deps .`

### Example usage:
```python
import alpenglow as ag
import alpenglow.experiments.FactorModelExperiment
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

data = pd.read_csv(
    "/mnt/idms/recSys/data/30M_lastfm/recoded_online_id_artist_first_filtered",
    sep=' ',
    header=None,
    names=['time', 'user', 'item', 'id', 'score', 'eval'],
    nrows=1000000
)
factorModelExperiment = FactorModelExperiment(
    topK=100,
    seed=254938879,
    dimension=10,
    learningRate=0.14,
    negativeRate=100
)
facRankings = factorModelExperiment.run(data, verbose=True)
facResults = ag.NdcgScore(facRankings)
facResults.timeFrame(60 * 60 * 24).plot()
plt.savefig("factor.png")
```